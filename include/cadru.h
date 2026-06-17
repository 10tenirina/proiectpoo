#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <istream>
#include "subiect_vizual.h"
#include "exceptii.h"

// forward declaration - fabrica e definita in cadru.cpp dupa includerea derivatelor
std::unique_ptr<SubiectVizual> creeazaSubiectDinStream(std::istream &is);

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

    // apeleaza contributieCompozitionala() virtual prin pointer de baza
    double calculeazaScorCompozitie() const;

    std::string interpreteazaScor() const;

    // STL: std::any_of
    bool areSuprapuneri() const;

    // apeleaza sfatCompozitional() si contributieCompozitionala() prin pointer de baza
    void raportDetaliat() const;

    // STL: std::max_element; arunca ExceptieScenaGoala daca e gol
    const SubiectVizual &protagonistul() const;

    friend std::istream &operator>>(std::istream &is, Cadru &c);

    friend std::ostream &operator<<(std::ostream &os, const Cadru &c);
};
