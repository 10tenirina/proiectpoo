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

// Tipul de compozitie dominant al unui cadru, dedus din pozitia
// protagonistului si din distributia subiectelor.
// Tipul subiacent explicit (int) permite forward-declararea enum-ului
// in stil_compozitional.h fara dependenta ciclica.
enum class TipCompozitie : int {
    Centrata, // protagonistul e in centrul cadrului (compozitie plata)
    ReguliTreimilor, // protagonistul cade pe/langa un power point
    Simetrica, // greutatea vizuala e echilibrata stanga-dreapta
    Echilibrata // niciuna dintre cele de mai sus, dar fara dezechilibru major
};

std::string descriereTipCompozitie(TipCompozitie tip);

// Distributia "greutatii vizuale" (suma importantelor) pe cele patru zone
// ale cadrului. Folosita pentru a evalua echilibrul compozitiei.
struct Echilibru {
    double stanga;
    double dreapta;
    double sus;
    double jos;

    // un cadru e echilibrat daca dezechilibrul relativ pe ambele axe e mic
    bool esteEchilibrat(double prag = 0.2) const;

    friend std::ostream &operator<<(std::ostream &os, const Echilibru &e);
};

// Shot-ul cinematografic.
// Contine subiecte vizuale prin pointer de baza (unique_ptr),
// ceea ce permite polimorfism si gestionare automata a memoriei.
//
// Deoarece are vector<unique_ptr<>>, unique_ptr nu e copiabil
// => definim explicit cc si op= folosind copy-and-swap + clone().
class Cadru {
    std::string titlu;
    double latime;
    double inaltime;
    std::vector<std::unique_ptr<SubiectVizual> > subiecte; // pointer la baza
    // Observer pattern: raw pointers, observatorii nu sunt detinuti de Cadru.
    // Lifetime-ul lor e gestionat in afara (vezi observator_cadru.h).
    std::vector<ObservatorCadru *> observatori;

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

    // Observer pattern: ataseaza/detaseaza ascultatori la modificarile cadrului.
    // Observatorii primesc notificari sincron, in ordinea atasarii.
    void adaugaObservator(ObservatorCadru *obs);

    void eliminaObservatori();

    // apeleaza contributieCompozitionala() virtual prin pointer de baza.
    // Foloseste stilul implicit (StilCinematic) - filozofie neutra.
    double calculeazaScorCompozitie() const;

    // Strategy: scorul agregat dupa filozofia unui stil cinematografic
    // (Hollywood clasic, Wes Anderson, documentar). Stilul moduleaza atat
    // ponderea scorului individual cat si ajustarea pe tipul de compozitie.
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

    // Statistici descriptive peste importantele subiectelor (clasa template).
    // Folosit pentru a evalua daca un cadru are subiecte "echivalente ca pondere"
    // sau daca exista un protagonist clar mai important decat restul.
    Statistici<int> statisticiImportanta() const;

    // Functie template: extrage din cadru toate subiectele de tipul dat,
    // intorse ca pointeri non-owning. Folosit pentru analize specifice pe
    // tipuri concrete (de ex. doar actorii sau doar sursele de lumina).
    //
    // Definita inline aici (functie template - instantiere la utilizare).
    // Cerere pe T: trebuie sa derive din SubiectVizual pentru ca dynamic_cast
    // sa fie posibil semantic (clasa polimorfica).
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