#include "scena.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>

Scena::Scena() : titlu{"scena necunoscuta"} {
}

Scena::Scena(const std::string &titlu_) : titlu{titlu_} {
}

const std::string &Scena::getTitlu() const { return titlu; }

void Scena::adaugaCadru(const Cadru &cadru) {
    cadre.push_back(cadru);
}

const Cadru &Scena::cadruRecomandat() const {
    if (cadre.empty())
        throw ExceptieScenaGoala("cadruRecomandat() apelat pe scena fara cadre");
    auto it = std::max_element(cadre.begin(), cadre.end(),
                               [](const Cadru &a, const Cadru &b) {
                                   return a.calculeazaScorCompozitie() < b.calculeazaScorCompozitie();
                               });
    return *it;
}

double Scena::scorMediu() const {
    if (cadre.empty()) return 0.0;
    const double total = std::accumulate(cadre.begin(), cadre.end(), 0.0,
                                         [](double suma, const Cadru &c) {
                                             return suma + c.calculeazaScorCompozitie();
                                         });
    return total / static_cast<double>(cadre.size());
}

void Scena::afiseazaRaport() const {
    std::cout << "~~~ Raport scena: \"" << titlu << "\" ~~~\n";
    for (const auto &c: cadre) {
        std::cout << "  \"" << c.getTitlu() << "\""
                << " | scor: " << c.calculeazaScorCompozitie() << "/100"
                << " | " << c.interpreteazaScor() << "\n";
        if (c.areSuprapuneri())
            std::cout << "    ! Atentie: exista suprapuneri intre subiecte\n";
    }
    std::cout << "  Scor mediu: " << scorMediu() << "/100\n";
    std::cout << "  Cadru recomandat: \""
            << cadruRecomandat().getTitlu() << "\"\n\n";
}

Scena Scena::dinFisier(const std::string &numeFisier) {
    std::ifstream f(numeFisier);
    if (!f.is_open())
        throw ExceptieFisierInvalid(numeFisier, "nu poate fi deschis");
    Scena s{};
    f >> s;
    if (f.fail() && !f.eof())
        throw ExceptieFisierInvalid(numeFisier, "eroare la citire - format incorect");
    return s;
}

std::istream &operator>>(std::istream &is, Scena &s) {
    int n = 0;
    is >> s.titlu >> n;
    s.cadre.clear();
    for (int i = 0; i < n; ++i) {
        Cadru c{};
        is >> c;
        s.cadre.push_back(c);
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const Scena &s) {
    os << "~~~ Scena: \"" << s.titlu << "\" ("
            << s.cadre.size() << " cadre) ~~~\n";
    for (const auto &c: s.cadre)
        os << c; // Cadru::operator<< -> SubiectVizual NVI -> afiseazaDetalii() virtual
    return os;
}
