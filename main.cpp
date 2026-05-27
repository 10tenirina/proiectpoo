#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "exceptii.h"
#include "punct.h"
#include "subiect_vizual.h"
#include "actor.h"
#include "recuzita.h"
#include "decor.h"

// Fabrica de subiecte vizuale: citeste din stream tipul si campurile specifice,
// returneaza un unique_ptr la derivata corecta.
// Separata de Cadru ca sa nu faca clasa prea mare.
// Format: denumire x y latime inaltime tip importanta [camp_specific_tipului]
//   Actor:   + directiePrivire  ("stanga" | "dreapta" | "camera")
//   Recuzita: + mobilitate      ("static" | "dinamic")
//   Decor:   + tipDecor         ("arhitectural" | "mobilier" | "vegetal" | ...)
std::unique_ptr<SubiectVizual> creeazaSubiectDinStream(std::istream& is);

// ============================================================
// Cadru - shot-ul cinematografic.
// Contine subiecte vizuale prin pointer de baza (unique_ptr),
// ceea ce permite polimorfism si gestionare automata a memoriei.
//
// Deoarece are vector<unique_ptr<>>, unique_ptr nu e copiabil
// => definim explicit cc si op= folosind copy-and-swap + clone().
// ============================================================
class Cadru {
    std::string                              titlu;
    double                                   latime;
    double                                   inaltime;
    std::vector<std::unique_ptr<SubiectVizual>> subiecte;   // pointer la baza

public:
    Cadru() : titlu{"cadru necunoscut"}, latime{1920.0}, inaltime{1080.0} {}

    explicit Cadru(const std::string& titlu_, double latime_, double inaltime_)
        : titlu{titlu_}, latime{latime_}, inaltime{inaltime_} {
        if(latime_ <= 0.0)
            throw ExceptieCadruInvalid("latime invalida (" + std::to_string(latime_) + ")");
        if(inaltime_ <= 0.0)
            throw ExceptieCadruInvalid("inaltime invalida (" + std::to_string(inaltime_) + ")");
    }

    // copy constructor: cloneaza fiecare subiect prin interfata virtuala
    Cadru(const Cadru& other)
        : titlu{other.titlu}, latime{other.latime}, inaltime{other.inaltime} {
        for(const auto& sv : other.subiecte)
            subiecte.push_back(sv->clone());
    }

    // copy-and-swap: op= primeste copia prin valoare, face swap cu this
    friend void swap(Cadru& a, Cadru& b) noexcept {
        using std::swap;
        swap(a.titlu,    b.titlu);
        swap(a.latime,   b.latime);
        swap(a.inaltime, b.inaltime);
        swap(a.subiecte, b.subiecte);
    }

    Cadru& operator=(Cadru other) {   // other e deja o copie (cc apelat)
        swap(*this, other);
        return *this;
    }

    ~Cadru() = default;   // unique_ptr elibereaza memoria automat

    const std::string& getTitlu() const { return titlu; }

    void adaugaSubiect(std::unique_ptr<SubiectVizual> subiect) {
        subiecte.push_back(std::move(subiect));
    }

    // scor compozitie 0-100, ponderat cu importanta;
    // apeleaza contributieCompozitionala() virtual prin pointer de baza
    double calculeazaScorCompozitie() const {
        if(subiecte.empty()) return 0.0;

        double scorTotal    = 0.0;
        double ponderaTotal = 0.0;

        for(const auto& sv : subiecte) {
            // apel virtual: Actor / Recuzita / Decor calculeaza diferit
            const double scorSubiect = sv->contributieCompozitionala(latime, inaltime);
            const double pondere     = static_cast<double>(sv->getImportanta());
            scorTotal    += scorSubiect * pondere;
            ponderaTotal += pondere;
        }
        return scorTotal / ponderaTotal;
    }

    std::string interpreteazaScor() const {
        const double scor = calculeazaScorCompozitie();
        if(scor >= 80.0) return "Compozitie excelenta - rule of thirds respectata";
        if(scor >= 55.0) return "Compozitie buna - elemente aproape de liniile de treimi";
        if(scor >= 30.0) return "Compozitie medie - elemente partial aliniate";
        return                  "Compozitie slaba - elemente departe de liniile de treimi";
    }

    bool areSuprapuneri() const {
        for(std::size_t i = 0; i < subiecte.size(); ++i)
            for(std::size_t j = i + 1; j < subiecte.size(); ++j)
                if(subiecte[i]->seSuprapuneCu(*subiecte[j]))
                    return true;
        return false;
    }

    // returneaza protagonistul (importanta maxima) prin pointer de baza
    // STL: std::max_element cu lambda
    const SubiectVizual& protagonistul() const {
        if(subiecte.empty())
            throw ExceptieScenaGoala("protagonistul() apelat pe cadru fara subiecte");
        auto it = std::max_element(subiecte.begin(), subiecte.end(),
            [](const auto& a, const auto& b) {
                return a->getImportanta() < b->getImportanta();
            });
        return **it;   // it -> unique_ptr -> SubiectVizual&
    }

    // format fisier: titlu latime inaltime n, urmat de n subiecte
    friend std::istream& operator>>(std::istream& is, Cadru& c) {
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

    // compunere de apeluri:
    // Cadru::op<< -> SubiectVizual::op<< -> afiseazaDetalii() virtual
    friend std::ostream& operator<<(std::ostream& os, const Cadru& c) {
        os << "=== Cadru: \"" << c.titlu << "\" ("
           << c.latime << "x" << c.inaltime << "px) ===\n";
        os << "Subiecte vizuale (" << c.subiecte.size() << "):\n";
        for(const auto& sv : c.subiecte)
            os << "  " << *sv << "\n";   // -> SubiectVizual::operator<<
        return os;
    }
};

// ============================================================
// Scena - secventa de cadre pentru aceeasi scena dintr-un film.
// ============================================================
class Scena {
    std::string        titlu;
    std::vector<Cadru> cadre;

public:
    Scena() : titlu{"scena necunoscuta"} {}

    explicit Scena(const std::string& titlu_) : titlu{titlu_} {}

    const std::string& getTitlu() const { return titlu; }

    void adaugaCadru(const Cadru& cadru) {
        cadre.push_back(cadru);
    }

    // cadrul cu cel mai bun scor; STL: std::max_element cu lambda
    const Cadru& cadruRecomandat() const {
        if(cadre.empty())
            throw ExceptieScenaGoala("cadruRecomandat() apelat pe scena fara cadre");
        auto it = std::max_element(cadre.begin(), cadre.end(),
            [](const Cadru& a, const Cadru& b) {
                return a.calculeazaScorCompozitie() < b.calculeazaScorCompozitie();
            });
        return *it;
    }

    double scorMediu() const {
        if(cadre.empty()) return 0.0;
        double total = 0.0;
        for(const auto& c : cadre)
            total += c.calculeazaScorCompozitie();
        return total / static_cast<double>(cadre.size());
    }

    void afiseazaRaport() const {
        std::cout << "~~~ Raport scena: \"" << titlu << "\" ~~~\n";
        for(const auto& c : cadre) {
            std::cout << "  \"" << c.getTitlu() << "\""
                      << " | scor: " << c.calculeazaScorCompozitie() << "/100"
                      << " | " << c.interpreteazaScor() << "\n";
            if(c.areSuprapuneri())
                std::cout << "    ! Atentie: exista suprapuneri intre subiecte\n";
        }
        std::cout << "  Scor mediu: " << scorMediu() << "/100\n";
        std::cout << "  Cadru recomandat: \""
                  << cadruRecomandat().getTitlu() << "\"\n\n";
    }

    friend std::istream& operator>>(std::istream& is, Scena& s) {
        int n = 0;
        is >> s.titlu >> n;
        s.cadre.clear();
        for(int i = 0; i < n; ++i) {
            Cadru c{};
            is >> c;
            s.cadre.push_back(c);
        }
        return is;
    }

    static Scena dinFisier(const std::string& numeFisier) {
        std::ifstream f(numeFisier);
        if(!f.is_open())
            throw ExceptieFisierInvalid(numeFisier, "nu poate fi deschis");
        Scena s{};
        f >> s;
        if(f.fail() && !f.eof())
            throw ExceptieFisierInvalid(numeFisier, "eroare la citire - format incorect");
        return s;
    }

    // compunere de apeluri: Scena -> Cadru -> SubiectVizual -> Punct
    friend std::ostream& operator<<(std::ostream& os, const Scena& s) {
        os << "~~~ Scena: \"" << s.titlu << "\" ("
           << s.cadre.size() << " cadre) ~~~\n";
        for(const auto& c : s.cadre)
            os << c;   // -> Cadru::operator<<
        return os;
    }
};

// ============================================================
// Definitia fabricii - dupa toate clasele derivate
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

// ============================================================
// main
// ============================================================
int main() {
    std::cout << "======= Rule of Thirds Analyzer =======\n\n";

    try {
        Scena scena = Scena::dinFisier("scena.txt");
        std::cout << scena;
        scena.afiseazaRaport();

        // apeluri directe - nu doar prin afiseazaRaport
        std::cout << "Scor mediu scena \"" << scena.getTitlu() << "\": "
                  << scena.scorMediu() << "/100\n";
        std::cout << "Cadru recomandat: \""
                  << scena.cadruRecomandat().getTitlu() << "\"\n\n";

        // dynamic_cast cu sens: daca protagonistul primului cadru e Actor,
        // afisam directia privirii (info disponibila doar pe Actor)
        // Nota: cadruRecomandat() poate fi orice cadru, deci folosim primul
        // cadru incarcat, care e accesibil via scena afisata mai sus.
        // Construim un cadru de test pentru a demonstra downcast-ul.
        Cadru cadruTest("Test_downcast", 1920.0, 1080.0);
        cadruTest.adaugaSubiect(
            std::make_unique<Actor>("Pacino", Punct{640.0, 360.0}, 140.0, 220.0, 9, "dreapta"));
        cadruTest.adaugaSubiect(
            std::make_unique<Recuzita>("Ceasca", Punct{1280.0, 720.0}, 40.0, 40.0, 3, true));

        const SubiectVizual& prot = cadruTest.protagonistul();
        // downcast: verificam daca protagonistul e Actor pentru info specifice
        const Actor* actorPtr = dynamic_cast<const Actor*>(&prot);
        if(actorPtr != nullptr) {
            std::cout << "Protagonistul \"" << actorPtr->getDenumire()
                      << "\" este Actor si priveste spre: "
                      << actorPtr->getDirectiePrivire() << "\n";
        }
        std::cout << "Total subiecte create: "
                  << SubiectVizual::getNumarSubiecteCreate() << "\n\n";
    }
    catch(const ExceptieFisierInvalid& e) {
        std::cout << "[Eroare fisier] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieCadruInvalid& e) {
        std::cout << "[Eroare cadru] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieSubiectInvalid& e) {
        std::cout << "[Eroare subiect] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieScenaGoala& e) {
        std::cout << "[Eroare scena] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieRuleOfThirds& e) {
        std::cout << "[Eroare] " << e.what() << "\n";
        return 1;
    }

    // test copy-and-swap pe Cadru + folosire adaugaCadru pe Scena
    std::cout << "--- Test copy-and-swap Cadru ---\n";
    Cadru c1("Cadru_test_copiere", 1920.0, 1080.0);
    c1.adaugaSubiect(
        std::make_unique<Actor>("TestActor", Punct{640.0, 360.0}, 100.0, 200.0, 7, "stanga"));
    Cadru c2{c1};          // copy constructor: cloneaza subiectele prin clone()
    std::cout << "cc: cadru copiat, scor original=" << c1.calculeazaScorCompozitie()
              << " scor copie=" << c2.calculeazaScorCompozitie() << "\n";

    Cadru c3("Cadru_alt", 1280.0, 720.0);
    c3 = c1;               // operator= copy-and-swap
    std::cout << "op=: dupa atribuire, scor=" << c3.calculeazaScorCompozitie() << "\n";

    // adaugaCadru folosita explicit: construim o scena din cadre create manual
    Scena scenaTest{"Scena_construita_manual"};
    scenaTest.adaugaCadru(c1);
    scenaTest.adaugaCadru(c3);
    std::cout << "Scena manuala cu " << "2 cadre"
              << ", scor mediu: " << scenaTest.scorMediu() << "/100\n";

    std::cout << "\n======= Analiza finalizata. =======\n";
    return 0;
}