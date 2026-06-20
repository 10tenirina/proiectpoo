#pragma once
#include "subiect_vizual.h"
#include <memory>
#include <string>

// element focal de tip sursa de lumina vizibila in cadru
// (fereastra, veioza, felinar)
// sursa de lumina contribuie la compoz in doua feluri
//   - prin poz luminii in cadru: plasat pe o linie de
//     treime, devine un leading element care incadreaza scena
//   - prin directia luminii: lumina laterala sau de contur (rim) modeleaza
//     volumele si da profunzime, in timp ce lumina frontala aplatizeaza imaginea
class SursaLumina : public SubiectVizual {
    std::string directie; // "laterala" / "frontala" / "contra"

    void afiseazaDetalii(std::ostream &os) const override;

public:
    explicit SursaLumina(const std::string &denumire_,
                         const Punct &colt_,
                         double latime_,
                         double inaltime_,
                         int importanta_,
                         const std::string &directie_);

    std::unique_ptr<SubiectVizual> clone() const override;

    double contributieCompozitionala(double W, double H) const override;

    std::string sfatCompozitional() const override;
};
