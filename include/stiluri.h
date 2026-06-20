#pragma once
#include "stil_compozitional.h"

// Cele patru filozofii cinematografice concrete folosite in proiect.
//
// Sunt grupate intr-un singur fisier deoarece au structura comuna,
// nu evolueaza independent si sunt scurte. Daca aparitia unui nou stil
// implica o logica semnificativ mai complexa (de exemplu citire dintr-un
// fisier de configurare), poate fi mutat intr-un fisier propriu.
//
// Toate stilurile pondereaza neutru scorul individual al subiectelor
// (cu exceptia documentarului) si aplica filozofia proprie pe tipul
// dominant de compozitie al cadrului.

// Stilul implicit: rule of thirds neutra, fara ponderi sau bonusuri.
// Echivalent cu logica preexistenta a Cadrului.
class StilCinematic : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilCinematic();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};

// Hollywood "studio system": rule of thirds stricta, centrarea e penalizata
// dur. Compozitia trebuie sa fie sigura si lizibila pentru audienta larga.
class StilHollywoodClasic : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilHollywoodClasic();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};

// Wes Anderson: simetria centrala e ideala, rule of thirds clasica e
// considerata plata si lipsita de viziune autoriala.
class StilWesAnderson : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilWesAnderson();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};

// Documentar (cinema verite): toleranta mare la abateri. Aplatizeaza
// scorurile individuale spre mediu si acorda un mic bonus uniform pe
// tipul de compozitie - autenticitatea primeaza in fata compozitiei.
class StilDocumentar : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilDocumentar();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};