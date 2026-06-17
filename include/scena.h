#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include "cadru.h"
#include "exceptii.h"

// Secventa de cadre pentru aceeasi scena dintr-un film.
// Permite compararea variantelor de cadru si alegerea celei mai bune compozitii.
class Scena {
    std::string        titlu;
    std::vector<Cadru> cadre;

public:
    Scena();
    explicit Scena(const std::string& titlu_);

    const std::string& getTitlu() const;

    void adaugaCadru(const Cadru& cadru);

    // STL: std::max_element; arunca ExceptieScenaGoala daca e goala
    const Cadru& cadruRecomandat() const;

    // STL: std::accumulate
    double scorMediu() const;

    void afiseazaRaport() const;

    static Scena dinFisier(const std::string& numeFisier);

    friend std::istream& operator>>(std::istream& is, Scena& s);
    friend std::ostream& operator<<(std::ostream& os, const Scena& s);
};