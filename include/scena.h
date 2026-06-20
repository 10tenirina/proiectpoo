#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <ostream>
#include <istream>
#include "cadru.h"
#include "subiect_vizual.h"
#include "exceptii.h"
#include "stil_compozitional.h"
#include "statistici.h"

class ObservatorCadru;

// secventa de cadre pentru aceeasi scena
// permite compararea variantelor de cadru si alegerea celei mai bune compozitii
class Scena {
    std::string titlu;
    std::vector<Cadru> cadre;

public:
    Scena();

    explicit Scena(const std::string &titlu_);

    const std::string &getTitlu() const;

    void adaugaCadru(const Cadru &cadru);

    // STL: std::max_element; arunca ExceptieScenaGoala daca e goala
    const Cadru &cadruRecomandat() const;

    // STL: std::accumulate
    double scorMediu() const;

    void afiseazaRaport() const;

    // STL: std::sort; afiseaza cadrele ordonate descrescator dupa scor
    void afiseazaClasament() const;

    // operatii indexate folosite de modul interactiv; arunca daca indexul e invalid
    std::size_t numarCadre() const;

    void afiseazaAnalizaCadru(std::size_t index) const;

    void comparaCadre(std::size_t i, std::size_t j) const;

    void adaugaSubiectLaCadru(std::size_t index, std::unique_ptr<SubiectVizual> subiect);

    // strategy: afiseaza scorul cadrului dat sub fiecare stil cinematografic
    // din lista, arata diferente intre tipuri dif de compozitii
    // Primeste pointeri non-owning (consistent cu RegistruStiluri::toateStilurile).
    void evalueazaCadruCuStiluri(
        std::size_t index,
        const std::vector<const StilCompozitional *> &stiluri) const;

    // observer: ataseaza acelasi observator la fiecare cadru din scena
    // folosit la setup, inainte de a intra in modul interactiv, pentru ca
    // orice modificare a unui cadru sa fie raportata in jurnal sau monitor
    void adaugaObservatorLaToateCadrele(ObservatorCadru *obs);

    // statistici descriptive peste scorurile de compozitie
    // cls template: Statistici<double>, folosit pentru a evalua omogenitatea
    // scorurilor: o scena cu deviation mare are cadre inegale, una cu mediana
    // mare are cele mai multe cadre bune
    Statistici<double> statisticiScoruri() const;

    static Scena dinFisier(const std::string &numeFisier);

    friend std::istream &operator>>(std::istream &is, Scena &s);

    friend std::ostream &operator<<(std::ostream &os, const Scena &s);
};