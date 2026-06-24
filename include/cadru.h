#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <istream>
#include <type_traits>
#include "subiect_vizual.h"
#include "exceptii.h"
#include "statistici.h"

class StilCompozitional; // forward decl pentru Strategy pattern
class ObservatorCadru;   // forward decl pentru Observer pattern

// forward declaration - fabrica e definita in cadru.cpp dupa includerea derivatelor
std::unique_ptr<SubiectVizual> creeazaSubiectDinStream(std::istream &is);

// tipul de compozitie dominant al unui cadru, dedus din poz protagonistului si unde s puse subiectele
// tipul subiacent explicit (int) permite forward-declararea enum ului
// in stil_compozitional.h fara dependenta ciclica
enum class TipCompozitie : int {
    Centrata, // protagonist in centrul cadrului (compozitie plata)
    ReguliTreimilor, // protagonist cade pe/langa un power point
    Simetrica, // greutatea vizuala e equal stanga-dreapta
    Echilibrata // niciuna dintre cele de mai sus, dar fara dezechilibru major
};

std::string descriereTipCompozitie(TipCompozitie tip);

// distributia greutatii vizuale (suma importantelor) pe cele patru zone
// ale cadrului, folosita pentru a eval echilibrul compozitiei
struct Echilibru {
    double stanga;
    double dreapta;
    double sus;
    double jos;

    // un cadru e echilibrat daca dezechilibrul relativ pe ambele axe e mic
    bool esteEchilibrat(double prag = 0.2) const;

    friend std::ostream &operator<<(std::ostream &os, const Echilibru &e);
};

// contine subiecte vizuale prin pointer de baza (unique_ptr),
// ceea ce permite polimorfism + gestionare automata a mem
//
// ptc are vector<unique_ptr<>>, unique_ptr nu e copiabil
// => definim explicit cc si op= folosind copy-and-swap + clone().
class Cadru {
    std::string titlu;
    double latime;
    double inaltime;
    std::vector<std::unique_ptr<SubiectVizual> > subiecte; // pointer la baza
    // observer pattern: raw pointers, observatorii nu sunt detinuti de Cadru
    // lifetime ul e gestionat in afara (observator_cadru.h)
    std::vector<ObservatorCadru *> observatori;

    // penalty pe scor brut, indep de stil: suprapuneri si cadru prea incarcat
    double penalizariCompozitionale() const;

public:
    Cadru();

    explicit Cadru(const std::string &titlu_, double latime_, double inaltime_);

    // copy constructor: cloneaza fiecare subiect prin clone() virtual
    Cadru(const Cadru &other);

    // copy-and-swap
    friend void swap(Cadru &a, Cadru &b) noexcept;

    Cadru &operator=(Cadru other);

    ~Cadru() = default;

    const std::string &getTitlu() const;

    void adaugaSubiect(std::unique_ptr<SubiectVizual> subiect);

    // observer pattern: ataseaza/detaseaza ascultatori la modificarile cadrului
    // observatorii primesc notif sincron in ordinea atasarii
    void adaugaObservator(ObservatorCadru *obs);

    void eliminaObservatori();

    // apeleaza contributieCompozitionala() virtual prin pointer de baza
    // Foloseste stilul implicit (StilCinematic)
    double calculeazaScorCompozitie() const;

    // strategy: scorul agregat dupa filozofia unui stil cinematografic
    // (Hollywood clasic, Wes Anderson, documentar)
    // poti face ponderea scorului individual cat si ajustarea pe tipul de compozitie
    double calculeazaScorCompozitie(const StilCompozitional &stil) const;

    std::string interpreteazaScor() const;

    // STL: std::any_of
    bool areSuprapuneri() const;

    // apeleaza sfatCompozitional() si contributieCompozitionala() prin pointer de baza
    void raportDetaliat() const;

    // STL: std::max_element; arunca ExceptieScenaGoala daca e gol
    const SubiectVizual &protagonistul() const;

    // clasifica tipul de compozitie dominant pornind de la protagonist
    TipCompozitie tipCompozitie() const;

    // distributia greutatii vizuale pe cele patru zone ale cadrului
    Echilibru analizeazaEchilibru() const;

    // statistici descriptive peste importantele subiectelor (clasa template)
    // folosit pentru a evalua daca un cadru are subiecte echivalente ca pondere
    // sau daca exista un protagonist clar mai important decat restul
    Statistici<int> statisticiImportanta() const;

    // functie template: extrage din cadru toate subiectele de tipul dat,
    // intorse ca pointeri non-owning, folosit pentru analize specifice pe
    // tipuri concrete (de ex doar actorii sau doar sursele de lumina)

    // def inline aici (functie template - instantiere la utilizare)
    // cerere pe T: trebuie sa derive din SubiectVizual pentru ca dynamic_cast
    // sa fie posibil semantic (clasa polimorfica)
    template<typename T>
    std::vector<const T *> extragePeTip() const {
        static_assert(std::is_base_of_v<SubiectVizual, T>,
                      "extragePeTip<T> functioneaza doar pe derivatele lui SubiectVizual");
        std::vector<const T *> rezultat;
        for (const auto &sv: subiecte) {
            // dynamic_cast prin pointer de baza: intoarce nullptr daca tipul nu se potriveste
            if (const T *ptr = dynamic_cast<const T *>(sv.get()))
                rezultat.push_back(ptr);
        }
        return rezultat;
    }

    // raport comparativ side-by-side intre cadrul curent si altul
    void comparaCu(const Cadru &alt) const;

    friend std::istream &operator>>(std::istream &is, Cadru &c);

    friend std::ostream &operator<<(std::ostream &os, const Cadru &c);
};