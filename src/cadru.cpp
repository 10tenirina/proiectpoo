#include "cadru.h"
#include "actor.h"
#include "recuzita.h"
#include "decor.h"
#include "sursa_lumina.h"
#include "stil_compozitional.h"
#include "stiluri.h"
#include "observator_cadru.h"
#include <algorithm>
#include <cmath>
#include <iostream>

// ============================================================
// Cadru
// ============================================================

Cadru::Cadru()
    : titlu{"cadru necunoscut"}, latime{1920.0}, inaltime{1080.0} {
}

Cadru::Cadru(const std::string &titlu_, double latime_, double inaltime_)
    : titlu{titlu_}, latime{latime_}, inaltime{inaltime_} {
    if (latime_ <= 0.0)
        throw ExceptieCadruInvalid("latime invalida (" + std::to_string(latime_) + ")");
    if (inaltime_ <= 0.0)
        throw ExceptieCadruInvalid("inaltime invalida (" + std::to_string(inaltime_) + ")");
}

Cadru::Cadru(const Cadru &other)
    : titlu{other.titlu}, latime{other.latime}, inaltime{other.inaltime} {
    for (const auto &sv: other.subiecte)
        subiecte.push_back(sv->clone());
    // observatorii sunt atasati la un cadru "identitate" - nu se duplica odata cu datele
}

void swap(Cadru &a, Cadru &b) noexcept {
    using std::swap;
    swap(a.titlu, b.titlu);
    swap(a.latime, b.latime);
    swap(a.inaltime, b.inaltime);
    swap(a.subiecte, b.subiecte);
    // observatorii NU se schimba prin swap, combinat cu copy-and-swap,
    // asta inseamna ca `c1 = c2` pastreaza ascultatorii lui c1 si primeste
    // doar datele lui c2 - semantica intuitiva: observatorii sunt atasati
    // la cadrul "identitate", nu fac parte din datele lui
}

Cadru &Cadru::operator=(Cadru other) {
    swap(*this, other);
    return *this;
}

const std::string &Cadru::getTitlu() const { return titlu; }

void Cadru::adaugaSubiect(std::unique_ptr<SubiectVizual> subiect) {
    // mentinem un pointer la subiect inainte de move pentru notificare:
    // observatorii primesc o referinta la subiectul tocmai adaugat
    const SubiectVizual *ref = subiect.get();
    subiecte.push_back(std::move(subiect));
    // observer: notificare sincrona in ordinea atasarii
    // raw pointers - nu owns, doar dispatch
    for (ObservatorCadru *obs: observatori)
        if (obs != nullptr)
            obs->laAdaugareSubiect(*this, *ref);
}

void Cadru::adaugaObservator(ObservatorCadru *obs) {
    if (obs != nullptr)
        observatori.push_back(obs);
}

void Cadru::eliminaObservatori() {
    observatori.clear();
}

double Cadru::calculeazaScorCompozitie() const {
    // stilul default: cinematic neutru (prezerva comportamentul preexistent)
    // static const: instantiere unica, fara overhead la fiecare apel
    static const StilCinematic implicit;
    return calculeazaScorCompozitie(implicit);
}

double Cadru::calculeazaScorCompozitie(const StilCompozitional &stil) const {
    if (subiecte.empty()) return 0.0;
    double scorTotal = 0.0;
    double ponderaTotal = 0.0;
    for (const auto &sv: subiecte) {
        // apel virtual prin pointer de baza pentru scorul de baza
        const double scorBaza = sv->contributieCompozitionala(latime, inaltime);
        // stilul moduleaza scorul individual dupa filozofia proprie
        const double scorPonderat = stil.ponderaSubiect(*sv, scorBaza);
        const double pondere = sv->getImportanta();
        scorTotal += scorPonderat * pondere;
        ponderaTotal += pondere;
    }
    const double scorAgregat = scorTotal / ponderaTotal;
    // stilul aplica ultimul ajustaj in functie de tipul dominant de compozitie
    return stil.ajustarePentruTip(tipCompozitie(), scorAgregat);
}

std::string Cadru::interpreteazaScor() const {
    const double scor = calculeazaScorCompozitie();
    if (scor >= 80.0) return "Compozitie excelenta - rule of thirds respectata";
    if (scor >= 55.0) return "Compozitie buna - elemente aproape de liniile de treimi";
    if (scor >= 30.0) return "Compozitie medie - elemente partial aliniate";
    return "Compozitie slaba - elemente departe de liniile de treimi";
}

bool Cadru::areSuprapuneri() const {
    for (auto it = subiecte.begin(); it != subiecte.end(); ++it) {
        if (std::any_of(std::next(it), subiecte.end(),
                        [&it](const auto &alt) { return (*it)->seSuprapuneCu(*alt); }))
            return true;
    }
    return false;
}

void Cadru::raportDetaliat() const {
    std::cout << "--- Raport detaliat: \"" << titlu << "\" ---\n";
    for (const auto &sv: subiecte) {
        // apel virtual prin pointer de baza: fiecare derivata raspunde diferit
        std::cout << "  " << sv->getDenumire()
                << " | scor individual: "
                << sv->contributieCompozitionala(latime, inaltime) << "/100"
                << "\n    >> " << sv->sfatCompozitional() << "\n";
    }
    if (areSuprapuneri())
        std::cout << "  ! Atentie: exista suprapuneri intre subiecte\n";
}

const SubiectVizual &Cadru::protagonistul() const {
    if (subiecte.empty())
        throw ExceptieScenaGoala("protagonistul() apelat pe cadru fara subiecte");
    auto it = std::max_element(subiecte.begin(), subiecte.end(),
                               [](const auto &a, const auto &b) {
                                   return a->getImportanta() < b->getImportanta();
                               });
    return **it;
}

std::string descriereTipCompozitie(TipCompozitie tip) {
    switch (tip) {
        case TipCompozitie::Centrata:
            return "centrata (subiect in mijloc - compozitie plata)";
        case TipCompozitie::ReguliTreimilor:
            return "rule of thirds (subiect pe un power point)";
        case TipCompozitie::Simetrica:
            return "simetrica (greutate vizuala echilibrata stanga-dreapta)";
        case TipCompozitie::Echilibrata:
            return "echilibrata (fara aliniere clara, dar fara dezechilibru major)";
    }
    return "necunoscuta";
}

bool Echilibru::esteEchilibrat(double prag) const {
    const double totalOriz = stanga + dreapta;
    const double totalVert = sus + jos;
    if (totalOriz <= 0.0 || totalVert <= 0.0)
        return false;
    const double dezechilibruOriz = std::abs(stanga - dreapta) / totalOriz;
    const double dezechilibruVert = std::abs(sus - jos) / totalVert;
    return dezechilibruOriz < prag && dezechilibruVert < prag;
}

std::ostream &operator<<(std::ostream &os, const Echilibru &e) {
    os << "stanga=" << e.stanga << " dreapta=" << e.dreapta
            << " sus=" << e.sus << " jos=" << e.jos
            << (e.esteEchilibrat() ? " [echilibrat]" : " [dezechilibrat]");
    return os;
}

TipCompozitie Cadru::tipCompozitie() const {
    const Punct centru = protagonistul().getCentru(); // arunca daca e gol
    const double rx = centru.getX() / latime;
    const double ry = centru.getY() / inaltime;

    // aproape de centrul cadrului
    if (std::abs(rx - 0.5) < 0.08 && std::abs(ry - 0.5) < 0.08)
        return TipCompozitie::Centrata;

    // aproape de o intersectie de treimi (power point)
    const double linii[2] = {1.0 / 3.0, 2.0 / 3.0};
    for (const double lx: linii)
        for (const double ly: linii)
            if (std::abs(rx - lx) < 0.08 && std::abs(ry - ly) < 0.08)
                return TipCompozitie::ReguliTreimilor;

    // altfel, decidem dupa echilibrul vizual orizontal
    if (analizeazaEchilibru().esteEchilibrat())
        return TipCompozitie::Simetrica;

    return TipCompozitie::Echilibrata;
}

Echilibru Cadru::analizeazaEchilibru() const {
    Echilibru e{0.0, 0.0, 0.0, 0.0};
    for (const auto &sv: subiecte) {
        const double greutate = static_cast<double>(sv->getImportanta());
        const Punct centru = sv->getCentru();
        if (centru.getX() < latime / 2.0)
            e.stanga += greutate;
        else
            e.dreapta += greutate;
        if (centru.getY() < inaltime / 2.0)
            e.sus += greutate;
        else
            e.jos += greutate;
    }
    return e;
}

Statistici<int> Cadru::statisticiImportanta() const {
    // colectam importantele subiectelor in vector si construim Statistici<int>
    // daca cadrul e gol, vectorul e gol, statistici permite asta prin gol().
    std::vector<int> importante;
    importante.reserve(subiecte.size());
    for (const auto &sv: subiecte)
        importante.push_back(sv->getImportanta());
    return Statistici{std::move(importante)};
}

void Cadru::comparaCu(const Cadru &alt) const {
    std::cout << "--- Comparatie cadre ---\n";
    std::cout << "  A: \"" << titlu << "\" | scor "
            << calculeazaScorCompozitie() << "/100 | "
            << descriereTipCompozitie(tipCompozitie()) << "\n";
    std::cout << "     echilibru: " << analizeazaEchilibru() << "\n";
    std::cout << "  B: \"" << alt.titlu << "\" | scor "
            << alt.calculeazaScorCompozitie() << "/100 | "
            << descriereTipCompozitie(alt.tipCompozitie()) << "\n";
    std::cout << "     echilibru: " << alt.analizeazaEchilibru() << "\n";

    const double scorA = calculeazaScorCompozitie();
    const double scorB = alt.calculeazaScorCompozitie();
    if (scorA > scorB)
        std::cout << "  => \"" << titlu << "\" e mai bine compus (cu "
                << (scorA - scorB) << " puncte)\n";
    else if (scorB > scorA)
        std::cout << "  => \"" << alt.titlu << "\" e mai bine compus (cu "
                << (scorB - scorA) << " puncte)\n";
    else
        std::cout << "  => cele doua cadre au compozitii echivalente\n";
}

std::istream &operator>>(std::istream &is, Cadru &c) {
    int n = 0;
    is >> c.titlu >> c.latime >> c.inaltime >> n;
    if (c.latime <= 0.0)
        throw ExceptieCadruInvalid(
            "latime invalida in fisier (" + std::to_string(c.latime) + ")");
    if (c.inaltime <= 0.0)
        throw ExceptieCadruInvalid(
            "inaltime invalida in fisier (" + std::to_string(c.inaltime) + ")");
    c.subiecte.clear();
    for (int i = 0; i < n; ++i)
        c.subiecte.push_back(creeazaSubiectDinStream(is));
    return is;
}

std::ostream &operator<<(std::ostream &os, const Cadru &c) {
    os << "=== Cadru: \"" << c.titlu << "\" ("
            << c.latime << "x" << c.inaltime << "px) ===\n";
    os << "Subiecte vizuale (" << c.subiecte.size() << "):\n";
    for (const auto &sv: c.subiecte)
        os << "  " << *sv << "\n"; // NVI -> afiseazaDetalii() virtual
    return os;
}


// Format: denumire x y latime inaltime tip importanta [camp_specific]
//   Actor + directiePrivire ("stanga" | "dreapta" | "camera")
//   Recuzita + mobilitate ("static" | "dinamic")
//   Decor + tipDecor ("arhitectural" | "mobilier" | ...)
//   SursaLumina + directie ("laterala" | "frontala" | "contra")

std::unique_ptr<SubiectVizual> creeazaSubiectDinStream(std::istream &is) {
    std::string denumire, tip;
    double x = 0.0, y = 0.0, latime = 0.0, inaltime = 0.0;
    int importanta = 0;

    is >> denumire >> x >> y >> latime >> inaltime >> tip >> importanta;
    const Punct colt{x, y};

    if (tip == "Actor") {
        std::string directie;
        is >> directie;
        return std::make_unique<Actor>(denumire, colt, latime, inaltime, importanta, directie);
    }
    if (tip == "Recuzita") {
        std::string mobilitate;
        is >> mobilitate;
        return std::make_unique<Recuzita>(
            denumire, colt, latime, inaltime, importanta, mobilitate == "static");
    }
    if (tip == "Decor") {
        std::string tipDecor;
        is >> tipDecor;
        return std::make_unique<Decor>(denumire, colt, latime, inaltime, importanta, tipDecor);
    }
    if (tip == "SursaLumina") {
        std::string directie;
        is >> directie;
        return std::make_unique<SursaLumina>(denumire, colt, latime, inaltime, importanta, directie);
    }
    throw ExceptieSubiectInvalid("tip de subiect necunoscut: \"" + tip + "\"");
}