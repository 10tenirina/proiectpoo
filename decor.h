#pragma once
#include "subiect_vizual.h"
#include <memory>
#include <string>

// Element focal de tip scenografic (decor, arhitectura, mobilier).
// Decorul contribuie la compozitie prin leading lines si frame-within-frame:
// daca bounding box-ul sau taie o linie de treime, ghideaza privirea
// spre subiectul principal => bonus la scor.
class Decor : public SubiectVizual {
    std::string tipDecor; // "arhitectural" | "mobilier" | "vegetal" | etc.

    void afiseazaDetalii(std::ostream &os) const override;

public:
    explicit Decor(const std::string &denumire_,
                   const Punct &colt_,
                   double latime_,
                   double inaltime_,
                   int importanta_,
                   const std::string &tipDecor_);

    std::unique_ptr<SubiectVizual> clone() const override;

    double contributieCompozitionala(double W, double H) const override;

    std::string sfatCompozitional() const override;
};
