#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

// Punct - coordonate (x, y) in cadrul filmului
// SubiectVizual "are" un Punct de origine => compunere

class Punct {
    double x;
    double y;
public:
    Punct() : x{0.0}, y{0.0} {}

    explicit Punct(double x_, double y_) : x{x_}, y{y_} {}

    //apar in calcule din SubiectVizual
    double getX() const { return x; }
    double getY() const { return y; }

    double distantaFata(const Punct& alt) const {
        const double dx = x - alt.x;
        const double dy = y - alt.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool operator==(const Punct& alt) const {
        return x == alt.x && y == alt.y;
    }

    bool operator!=(const Punct& alt) const {
        return !(*this == alt);
    }

    friend std::istream& operator>>(std::istream& is, Punct& p) {
        is >> p.x >> p.y;
        return is;
    }

    // friend: acces direct la private, no getteri hah
    friend std::ostream& operator<<(std::ostream& os, const Punct& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

//SubiectVizual - element focal dintr-un cadru cinematografic
// (actor, obiect important, linia orizontului etc)
// bounding box: colt stanga-sus + latime + inaltime.
//
//un Cadru practic are mai multe SubiecteVizuale => compunere
//regula aplicata aici
class SubiectVizual {
    std::string denumire;
    Punct coltStangaSus;
    double latime;
    double inaltime;
    int importanta;         // 1 - 10

    // aria bounding box-ului - folosita in seSuprapuneCu
    // pentru a ignora subiectele cu dimensiuni zero
    double arie() const { return latime * inaltime; }

public:
    SubiectVizual() : denumire{"necunoscut"}, coltStangaSus{}, latime{0.0}, inaltime{0.0}, importanta{1} {
        std::cout << "Constr implicit SubiectVizual\n";
    }

    explicit SubiectVizual(const std::string& denumire_,
                           const Punct& colt_,
                           double latime_,
                           double inaltime_,
                           int importanta_ = 5)
        : denumire{denumire_},
          coltStangaSus{colt_},
          latime{latime_},
          inaltime{inaltime_},
          importanta{importanta_} {
        std::cout << "Constr de initializare SubiectVizual: " << denumire << "\n";
    }

    //rule of three

    SubiectVizual(const SubiectVizual& other)
        : denumire{other.denumire},
          coltStangaSus{other.coltStangaSus},
          latime{other.latime},
          inaltime{other.inaltime},
          importanta{other.importanta} {
        std::cout << "Constr de copiere SubiectVizual: " << denumire << "\n";
    }

    SubiectVizual& operator=(const SubiectVizual& other) {
        std::cout << "operator= copiere SubiectVizual: " << other.denumire << "\n";
        if(this != &other) {
            denumire      = other.denumire;
            coltStangaSus = other.coltStangaSus;
            latime        = other.latime;
            inaltime      = other.inaltime;
            importanta    = other.importanta;
        }
        return *this;
    }

    ~SubiectVizual() {
        std::cout << "Destr SubiectVizual: " << denumire << "\n";
    }

    // getteri strict necesari (folositi in Cadru pentru calcule)
    const std::string& getDenumire() const { return denumire; }
    int getImportanta() const { return importanta; }

    //centrul geometric al bounding box ului
    Punct getCentru() const {
        return Punct{coltStangaSus.getX() + latime / 2.0,
                     coltStangaSus.getY() + inaltime / 2.0};
    }

    // dist centru subiectului - power point
    // (power points = cele 4 intersectii ale liniilor care sunt la o treime
    //   (W/3, H/3), (2W/3, H/3), (W/3, 2H/3), (2W/3, 2H/3)
    double distantaFataDePowerPoint(double latimeCadru, double inaltimeCadru) const {
        const Punct centru = getCentru();
        double distMin = -1.0;
        const double fractii[2] = {1.0 / 3.0, 2.0 / 3.0};
        for(double fx : fractii) {
            for(double fy : fractii) {
                const Punct pp{fx * latimeCadru, fy * inaltimeCadru};
                const double d = centru.distantaFata(pp);
                if(distMin < 0.0 || d < distMin)
                    distMin = d;
            }
        }
        return distMin;
    }

    // verifica daca centrul subiectului e pe o linie
    // de treime (orizontala sau verticala)
    bool esteAliniat(double latimeCadru, double inaltimeCadru,
                     double toleranta = 0.05) const {
        const Punct centru = getCentru();
        const double rx = centru.getX() / latimeCadru;
        const double ry = centru.getY() / inaltimeCadru;
        const double linii[2] = {1.0 / 3.0, 2.0 / 3.0};
        for(double l : linii) {
            if(std::abs(rx - l) <= toleranta) return true;
            if(std::abs(ry - l) <= toleranta) return true;
        }
        return false;
    }

    // verifica daca acest subiect se suprapune spatial cu altul
    // subiectele cu arie zero sunt ignorate
    bool seSuprapuneCu(const SubiectVizual& alt) const {
        if(arie() == 0.0 || alt.arie() == 0.0) return false;
        const bool separatX =
            (coltStangaSus.getX() + latime      <= alt.coltStangaSus.getX())
         || (alt.coltStangaSus.getX() + alt.latime <= coltStangaSus.getX());
        const bool separatY =
            (coltStangaSus.getY() + inaltime       <= alt.coltStangaSus.getY())
         || (alt.coltStangaSus.getY() + alt.inaltime <= coltStangaSus.getY());
        return !separatX && !separatY;
    }

    bool operator==(const SubiectVizual& alt) const {
        return denumire      == alt.denumire
            && coltStangaSus == alt.coltStangaSus
            && latime        == alt.latime
            && inaltime      == alt.inaltime
            && importanta    == alt.importanta;
    }

    bool operator!=(const SubiectVizual& alt) const {
        return !(*this == alt);
    }

    // format: denumire x y latime inaltime importanta
    friend std::istream& operator>>(std::istream& is, SubiectVizual& sv) {
        double x = 0.0;
        double y = 0.0;
        is >> sv.denumire >> x >> y >> sv.latime >> sv.inaltime >> sv.importanta;
        sv.coltStangaSus = Punct{x, y};
        return is;
    }

    // friend: acces direct la private, exact ca in README
    // compunere de apeluri: apeleaza Punct::operator<<
    friend std::ostream& operator<<(std::ostream& os, const SubiectVizual& sv) {
        os << sv.denumire
           << " | colt: " << sv.coltStangaSus   // -> Punct::operator<<
           << " | " << sv.latime << "x" << sv.inaltime
           << " | importanta: " << sv.importanta << "/10";
        return os;
    }
};

// Cadru - shot-ul / scena filmului.
// contine mai multe SubiecteVizuale si dimensiunile cadrului
// un Cadru "are" SubiecteVizuale => compunere cu vector
class Cadru {
    std::string titlu;
    double latime;
    double inaltime;
    std::vector<SubiectVizual> subiecte;   // compunere

    // diagonala cadrului - folosita pentru normalizarea scorului
    double diagonala() const {
        return std::sqrt(latime * latime + inaltime * inaltime);
    }

public:
    Cadru() : titlu{"cadru necunoscut"}, latime{1920.0}, inaltime{1080.0} {
        std::cout << "Constr implicit Cadru\n";
    }

    explicit Cadru(const std::string& titlu_, double latime_, double inaltime_)
        : titlu{titlu_}, latime{latime_}, inaltime{inaltime_} {
        std::cout << "Constr de initializare Cadru: " << titlu << "\n";
    }

    void adaugaSubiect(const SubiectVizual& subiect) {
        subiecte.push_back(subiect);
    }

    const std::string& getTitlu() const { return titlu; }

    // scor compozitie 0-100, ponderat cu importanta fiecarui subiect
    // foloseste diagonala() pentru normalizare
    double calculeazaScorCompozitie() const {
        if(subiecte.empty()) return 0.0;

        double scorTotal    = 0.0;
        double ponderaTotal = 0.0;

        for(const auto& sv : subiecte) {
            const double dist         = sv.distantaFataDePowerPoint(latime, inaltime);
            const double distRelativa = dist / diagonala();
            const double scorSubiect  = std::max(0.0, 100.0 - distRelativa * 100.0);
            const double pondere      = static_cast<double>(sv.getImportanta());
            scorTotal    += scorSubiect * pondere;
            ponderaTotal += pondere;
        }
        return scorTotal / ponderaTotal;
    }

    // interpreteaza textual scorul de compozitie
    std::string interpreteazaScor() const {
        const double scor = calculeazaScorCompozitie();
        if(scor >= 80.0) return "Compozitie excelenta - rule of thirds respectata";
        if(scor >= 55.0) return "Compozitie buna - elemente aproape de liniile de treimi";
        if(scor >= 30.0) return "Compozitie medie - elemente partial aliniate";
        return                  "Compozitie slaba - elemente departe de liniile de treimi";
    }

    // verifica daca exista suprapuneri intre subiectele vizuale
    bool areSuprapuneri() const {
        for(std::size_t i = 0; i < subiecte.size(); ++i)
            for(std::size_t j = i + 1; j < subiecte.size(); ++j)
                if(subiecte[i].seSuprapuneCu(subiecte[j]))
                    return true;
        return false;
    }

    // returneaza subiectul cu importanta cea mai mare (protagonistul)
    const SubiectVizual& protagonistul() const {
        const SubiectVizual* cel_mai_important = &subiecte[0];
        for(const auto& sv : subiecte)
            if(sv.getImportanta() > cel_mai_important->getImportanta())
                cel_mai_important = &sv;
        return *cel_mai_important;
    }

    // format: titlu latime inaltime n, apoi n subiecte
    friend std::istream& operator>>(std::istream& is, Cadru& c) {
        int n = 0;
        is >> c.titlu >> c.latime >> c.inaltime >> n;
        c.subiecte.clear();
        for(int i = 0; i < n; ++i) {
            SubiectVizual sv{};
            is >> sv;
            c.subiecte.push_back(sv);
        }
        return is;
    }

    // friend: acces direct la private
    // compunere de apeluri: apeleaza SubiectVizual::operator<<
    friend std::ostream& operator<<(std::ostream& os, const Cadru& c) {
        os << "=== Cadru: \"" << c.titlu << "\" ("
           << c.latime << "x" << c.inaltime << "px) ===\n";
        os << "Subiecte vizuale (" << c.subiecte.size() << "):\n";
        for(const auto& sv : c.subiecte)
            os << "  [" << sv << "]\n";   // -> SubiectVizual::operator<<
        return os;
    }
};

// Scena - o secventa de cadre pentru aceeasi scena dintr-un film.
// Regizorul adauga variante de cadru si alege cea mai buna compozitie.
// Scena "are" mai multe Cadre => compunere
class Scena {
    std::string titlu;
    std::vector<Cadru> cadre;   // compunere

public:
    Scena() : titlu{"scena necunoscuta"} {
        std::cout << "Constr implicit Scena\n";
    }

    explicit Scena(const std::string& titlu_) : titlu{titlu_} {
        std::cout << "Constr de initializare Scena: " << titlu << "\n";
    }

    void adaugaCadru(const Cadru& cadru) {
        cadre.push_back(cadru);
    }

    const std::string& getTitlu() const { return titlu; }

    // returneaza cadrul cu cel mai bun scor de compozitie
    const Cadru& cadruRecomandat() const {
        const Cadru* cel_mai_bun = &cadre[0];
        for(const auto& c : cadre)
            if(c.calculeazaScorCompozitie() > cel_mai_bun->calculeazaScorCompozitie())
                cel_mai_bun = &c;
        return *cel_mai_bun;
    }

    // scorul mediu al tuturor cadrelor din scena
    double scorMediu() const {
        if(cadre.empty()) return 0.0;
        double total = 0.0;
        for(const auto& c : cadre)
            total += c.calculeazaScorCompozitie();
        return total / static_cast<double>(cadre.size());
    }

    // raport complet: fiecare cadru cu scorul sau + recomandarea finala
    void afiseazaRaport() const {
        std::cout << "~~~ Raport scena: \"" << titlu << "\" ~~~\n";
        for(const auto& c : cadre) {
            std::cout << "  \"" << c.getTitlu() << "\""
                      << " | scor: " << c.calculeazaScorCompozitie() << "/100"
                      << " | " << c.interpreteazaScor() << "\n";
        }
        std::cout << "  Scor mediu: " << scorMediu() << "/100\n";
        std::cout << "  Cadru recomandat: \""
                  << cadruRecomandat().getTitlu() << "\"\n\n";
    }

    // compunere de apeluri: apeleaza Cadru::operator<<
    // care apeleaza SubiectVizual::operator<< care apeleaza Punct::operator<<
    friend std::ostream& operator<<(std::ostream& os, const Scena& s) {
        os << "~~~ Scena: \"" << s.titlu << "\" ("
           << s.cadre.size() << " cadre) ~~~\n";
        for(const auto& c : s.cadre)
            os << c;   // -> Cadru::operator<<
        return os;
    }
};

// regizorul compara variante de cadru pentru aceeasi scena si alege varianta cu scor mai bun
int main() {
    std::cout << "======= Rule of Thirds Analyzer =======\n";
    std::cout << "Scenariu: regizorul compara doua variante de cadru pentru a alege compozitia mai buna.\n\n";

    // constructorii impliciti si functiile publice care nu prea apar in scenariu
    std::cout << "--- Constructori impliciti si functii de baza ---\n";
    Punct originea{};
    SubiectVizual svGol{};
    Cadru cadrGol{};
    Scena scenaGoala{};
    std::cout << "Punct implicit: " << originea << "\n";
    std::cout << "SubiectVizual implicit: " << svGol << "\n";
    std::cout << "Cadru implicit - titlu: " << cadrGol.getTitlu() << "\n";
    std::cout << "Scena implicita - titlu: " << scenaGoala.getTitlu() << "\n";

    // Punct::distantaFata apelata explicit
    Punct pp1{640.0, 360.0};
    Punct pp2{1280.0, 720.0};
    std::cout << "Distanta intre power points diagonale: "
              << pp1.distantaFata(pp2) << "px\n\n";

    // cadru 1920x1080
    // linii de treimi: x=640, x=1280, y=360, y=720
    // power points:    (640,360), (1280,360), (640,720), (1280,720)
    const double W = 1920.0;
    const double H = 1080.0;

    // citire cadru de la tastatura (sau redirectionat din tastatura.txt)
    std::cout << "--- Cadru citit de la tastatura ---\n";
    std::cout << "(format: titlu latime inaltime n_subiecte\n";
    std::cout << "         apoi: denumire x y latime inaltime importanta)\n";
    Cadru cadrCitit{};
    std::cin >> cadrCitit;
    std::cout << cadrCitit;
    std::cout << "Scor: " << cadrCitit.calculeazaScorCompozitie() << "/100"
              << " | " << cadrCitit.interpreteazaScor() << "\n\n";

    //A: compozitie buna - placement pe power points
    std::cout << "--- Varianta A: compozitie intentionata ---\n";

    // protagonist in stanga-sus, aproape de power point (640, 360)
    SubiectVizual actorA{"Michael Corleone", Punct{570.0, 280.0}, 140.0, 220.0, 9};
    // recuzita importanta in dreapta-jos, aproape de power point (1280, 720)
    SubiectVizual recuzitaA{"Pistolul", Punct{1250.0, 690.0}, 60.0, 40.0, 8};
    // Personaj secundar in dreapta-sus, aproape de power point (1280, 360)
    SubiectVizual secundarA{"Fredo", Punct{1210.0, 270.0}, 100.0, 180.0, 5};

    std::cout << actorA << "\n";
    std::cout << recuzitaA << "\n";
    std::cout << secundarA << "\n\n";

    std::cout << "Analiza individuala varianta A:\n";
    std::cout << "  " << actorA.getDenumire()
              << " | centru: " << actorA.getCentru()
              << " | dist pp: " << actorA.distantaFataDePowerPoint(W, H) << "px"
              << " | aliniat: " << (actorA.esteAliniat(W, H) ? "Da" : "Nu") << "\n";
    std::cout << "  " << recuzitaA.getDenumire()
              << " | centru: " << recuzitaA.getCentru()
              << " | dist pp: " << recuzitaA.distantaFataDePowerPoint(W, H) << "px"
              << " | aliniat: " << (recuzitaA.esteAliniat(W, H) ? "Da" : "Nu") << "\n";
    std::cout << "  " << secundarA.getDenumire()
              << " | centru: " << secundarA.getCentru()
              << " | dist pp: " << secundarA.distantaFataDePowerPoint(W, H) << "px"
              << " | aliniat: " << (secundarA.esteAliniat(W, H) ? "Da" : "Nu") << "\n";
    std::cout << "  Suprapuneri: actor<->recuzita="
              << (actorA.seSuprapuneCu(recuzitaA) ? "Da" : "Nu")
              << ", actor<->secundar="
              << (actorA.seSuprapuneCu(secundarA) ? "Da" : "Nu") << "\n\n";

    Cadru variantaA{"The Godfather - varianta A", W, H};
    variantaA.adaugaSubiect(actorA);
    variantaA.adaugaSubiect(recuzitaA);
    variantaA.adaugaSubiect(secundarA);

    std::cout << variantaA;
    const double scorA = variantaA.calculeazaScorCompozitie();
    std::cout << "Scor: " << scorA << "/100 | " << variantaA.interpreteazaScor() << "\n";
    std::cout << "Suprapuneri in cadru: " << (variantaA.areSuprapuneri() ? "Da" : "Nu") << "\n";
    std::cout << "Protagonist: " << variantaA.protagonistul() << "\n\n";

    // B: compozitie slaba - subiecte plasate central/aleatoriu
    std::cout << "--- Varianta B: compozitie slaba (centrat) ---\n";

    // toate subiectele in zona centrala - rookie mistake.
    // centrul cadrului (960,540) este departe de toate cele 4 pp.
    SubiectVizual actorB{"Michael Corleone", Punct{890.0, 430.0}, 140.0, 220.0, 9};
    SubiectVizual recuzitaB{"Pistolul", Punct{1010.0, 610.0}, 60.0, 40.0, 8};
    SubiectVizual secundarB{"Fredo", Punct{750.0, 390.0}, 100.0, 180.0, 5};

    std::cout << actorB << "\n";
    std::cout << recuzitaB << "\n";
    std::cout << secundarB << "\n\n";

    std::cout << "Analiza individuala varianta B:\n";
    std::cout << "  " << actorB.getDenumire()
              << " | centru: " << actorB.getCentru()
              << " | dist pp: " << actorB.distantaFataDePowerPoint(W, H) << "px"
              << " | aliniat: " << (actorB.esteAliniat(W, H) ? "Da" : "Nu") << "\n";
    std::cout << "  " << recuzitaB.getDenumire()
              << " | centru: " << recuzitaB.getCentru()
              << " | dist pp: " << recuzitaB.distantaFataDePowerPoint(W, H) << "px"
              << " | aliniat: " << (recuzitaB.esteAliniat(W, H) ? "Da" : "Nu") << "\n";
    std::cout << "  " << secundarB.getDenumire()
              << " | centru: " << secundarB.getCentru()
              << " | dist pp: " << secundarB.distantaFataDePowerPoint(W, H) << "px"
              << " | aliniat: " << (secundarB.esteAliniat(W, H) ? "Da" : "Nu") << "\n";
    std::cout << "  Suprapuneri: actor<->recuzita="
              << (actorB.seSuprapuneCu(recuzitaB) ? "Da" : "Nu")
              << ", recuzita<->secundar="
              << (recuzitaB.seSuprapuneCu(secundarB) ? "Da" : "Nu") << "\n\n";

    Cadru variantaB{"The Godfather - varianta B", W, H};
    variantaB.adaugaSubiect(actorB);
    variantaB.adaugaSubiect(recuzitaB);
    variantaB.adaugaSubiect(secundarB);

    std::cout << variantaB;
    const double scorB = variantaB.calculeazaScorCompozitie();
    std::cout << "Scor: " << scorB << "/100 | " << variantaB.interpreteazaScor() << "\n";
    std::cout << "Suprapuneri in cadru: " << (variantaB.areSuprapuneri() ? "Da" : "Nu") << "\n";
    std::cout << "Protagonist: " << variantaB.protagonistul() << "\n\n";

    // scena care contine toate cadrele - regizorul vede imaginea de ansamblu
    Scena scena{"The Godfather - scena finala"};
    scena.adaugaCadru(variantaA);
    scena.adaugaCadru(variantaB);
    scena.adaugaCadru(cadrCitit);

    std::cout << scena;          // Scena -> Cadru -> SubiectVizual -> Punct
    scena.afiseazaRaport();

    // test regula celor trei
    std::cout << "--- Test regula celor trei ---\n";
    SubiectVizual sv1{"Original", Punct{100.0, 200.0}, 150.0, 100.0, 8};
    SubiectVizual sv2{sv1};
    assert((std::cout << "cc: Atributele se copiaza corect\n", sv1 == sv2));
    sv2 = SubiectVizual{"Modificat", Punct{999.0, 999.0}, 10.0, 10.0, 1};
    assert((std::cout << "cc: Modificarea copiei nu modifica originalul\n", sv1 != sv2));
    sv1 = sv2;
    assert((std::cout << "op=: Atributele se copiaza corect\n", sv1 == sv2));
    sv1 = SubiectVizual{"AltModificat", Punct{0.0, 0.0}, 5.0, 5.0, 2};
    assert((std::cout << "op=: Modificarea copiei nu modifica originalul\n", sv1 != sv2));

    std::cout << "\n======= Cadru analizat. =======\n";
    return 0;
}