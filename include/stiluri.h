#pragma once
#include "stil_compozitional.h"

// patru filozofii cinematografice concrete folosite de data asta
// daca aparitia unui nou stil implica o logica semnificativ mai complexa (de exemplu citire dintr un
// fisier de configurare), poate fi mutat intr un fisier separat

// toate stilurile pondereaza neutru scorul individual al subiectelor
// (cu exceptia documentarului) si aplica filozofia/regula proprie pe tipul
// dominant de compozitie al cadrului

// stilul default: rule of thirds neutra, fara ponderi sau bonusuri
// echiv cu logica preexistenta a Cadrului
class StilCinematic : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilCinematic();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};

// Hollywood "studio system": rule of thirds stricta, centrarea e penalizata
// serios, compozitia trebuie sa fie sigura si lizibila pentru audienta larga
class StilHollywoodClasic : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilHollywoodClasic();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};

// Wes Anderson: simetria centrala e ideala, rule of thirds clasica e
// considerata plata
class StilWesAnderson : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilWesAnderson();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};

// Documentar (cinema verite): toleranta mare la abateri, aplatizeaza
// scorurile individuale spre mediu si acorda un mic bonus uniform pe
// tipul de compozitie
class StilDocumentar : public StilCompozitional {
    void afiseazaDescriere(std::ostream &os) const override;

public:
    StilDocumentar();

    std::unique_ptr<StilCompozitional> clone() const override;

    double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const override;

    double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const override;
};