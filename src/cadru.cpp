#include "cadru.h"
#include "actor.h"
#include "recuzita.h"
#include "decor.h"
#include <algorithm>
#include <iostream>

// ============================================================
// Cadru
// ============================================================

Cadru::Cadru()
    : titlu{"cadru necunoscut"}, latime{1920.0}, inaltime{1080.0} {}

Cadru::Cadru(const std::string& titlu_, double latime_, double inaltime_)
    : titlu{titlu_}, latime{latime_}, inaltime{inaltime_} {
    if(latime_ <= 0.0)
        throw ExceptieCadruInvalid("latime invalida (" + std::to_string(latime_) + ")");
    if(inaltime_ <= 0.0)
        throw ExceptieCadruInvalid("inaltime invalida (" + std::to_string(inaltime_) + ")");
}

Cadru::Cadru(const Cadru& other)
    : titlu{other.titlu}, latime{other.latime}, inaltime{other.inaltime} {
    for(const auto& sv : other.subiecte)
        subiecte.push_back(sv->clone());
}

void swap(Cadru& a, Cadru& b) noexcept {
    using std::swap;
    swap(a.titlu,    b.titlu);
    swap(a.latime,   b.latime);
    swap(a.inaltime, b.inaltime);
    swap(a.subiecte, b.subiecte);
}

Cadru& Cadru::operator=(Cadru other) {
    swap(*this, other);
    return *this;
}

const std::string& Cadru::getTitlu() const { return titlu; }

void Cadru::adaugaSubiect(std::unique_ptr<SubiectVizual> subiect) {
    subiecte.push_back(std::move(subiect));
}

double Cadru::calculeazaScorCompozitie() const {
    if(subiecte.empty()) return 0.0;
    double scorTotal    = 0.0;
    double ponderaTotal = 0.0;
    for(const auto& sv : subiecte) {
        // apel virtual prin pointer de baza
        const double scorSubiect = sv->contributieCompozitionala(latime, inaltime);
        const double pondere     = static_cast<double>(sv->getImportanta());
        scorTotal    += scorSubiect * pondere;
        ponderaTotal += pondere;
    }
    return scorTotal / ponderaTotal;
}

std::string Cadru::interpreteazaScor() const {
    const double scor = calculeazaScorCompozitie();
    if(scor >= 80.0) return "Compozitie excelenta - rule of thirds respectata";
    if(scor >= 55.0) return "Compozitie buna - elemente aproape de liniile de treimi";
    if(scor >= 30.0) return "Compozitie medie - elemente partial aliniate";
    return                  "Compozitie slaba - elemente departe de liniile de treimi";
}

bool Cadru::areSuprapuneri() const {
    for(auto it = subiecte.begin(); it != subiecte.end(); ++it) {
        if(std::any_of(std::next(it), subiecte.end(),
            [&it](const auto& alt) { return (*it)->seSuprapuneCu(*alt); }))
            return true;
    }
    return false;
}

void Cadru::raportDetaliat() const {
    std::cout << "--- Raport detaliat: \"" << titlu << "\" ---\n";
    for(const auto& sv : subiecte) {
        // apel virtual prin pointer de baza: fiecare derivata raspunde diferit
        std::cout << "  " << sv->getDenumire()
                  << " | scor individual: "
                  << sv->contributieCompozitionala(latime, inaltime) << "/100"
                  << "\n    >> " << sv->sfatCompozitional() << "\n";
    }
    if(areSuprapuneri())
        std::cout << "  ! Atentie: exista suprapuneri intre subiecte\n";
}

const SubiectVizual& Cadru::protagonistul() const {
    if(subiecte.empty())
        throw ExceptieScenaGoala("protagonistul() apelat pe cadru fara subiecte");
    auto it = std::max_element(subiecte.begin(), subiecte.end(),
        [](const auto& a, const auto& b) {
            return a->getImportanta() < b->getImportanta();
        });
    return **it;
}

std::istream& operator>>(std::istream& is, Cadru& c) {
    int n = 0;
    is >> c.titlu >> c.latime >> c.inaltime >> n;
    if(c.latime <= 0.0)
        throw ExceptieCadruInvalid(
            "latime invalida in fisier (" + std::to_string(c.latime) + ")");
    if(c.inaltime <= 0.0)
        throw ExceptieCadruInvalid(
            "inaltime invalida in fisier (" + std::to_string(c.inaltime) + ")");
    c.subiecte.clear();
    for(int i = 0; i < n; ++i)
        c.subiecte.push_back(creeazaSubiectDinStream(is));
    return is;
}

std::ostream& operator<<(std::ostream& os, const Cadru& c) {
    os << "=== Cadru: \"" << c.titlu << "\" ("
       << c.latime << "x" << c.inaltime << "px) ===\n";
    os << "Subiecte vizuale (" << c.subiecte.size() << "):\n";
    for(const auto& sv : c.subiecte)
        os << "  " << *sv << "\n";   // NVI -> afiseazaDetalii() virtual
    return os;
}

// ============================================================
// Fabrica de subiecte vizuale
// Definita dupa includerea tuturor derivatelor.
// Format: denumire x y latime inaltime tip importanta [camp_specific]
//   Actor:    + directiePrivire  ("stanga" | "dreapta" | "camera")
//   Recuzita: + mobilitate       ("static" | "dinamic")
//   Decor:    + tipDecor         ("arhitectural" | "mobilier" | ...)
// ============================================================
std::unique_ptr<SubiectVizual> creeazaSubiectDinStream(std::istream& is) {
    std::string denumire, tip;
    double x = 0.0, y = 0.0, latime = 0.0, inaltime = 0.0;
    int importanta = 0;

    is >> denumire >> x >> y >> latime >> inaltime >> tip >> importanta;
    const Punct colt{x, y};

    if(tip == "Actor") {
        std::string directie;
        is >> directie;
        return std::make_unique<Actor>(denumire, colt, latime, inaltime, importanta, directie);
    }
    if(tip == "Recuzita") {
        std::string mobilitate;
        is >> mobilitate;
        return std::make_unique<Recuzita>(
            denumire, colt, latime, inaltime, importanta, mobilitate == "static");
    }
    if(tip == "Decor") {
        std::string tipDecor;
        is >> tipDecor;
        return std::make_unique<Decor>(denumire, colt, latime, inaltime, importanta, tipDecor);
    }
    throw ExceptieSubiectInvalid("tip de subiect necunoscut: \"" + tip + "\"");
}