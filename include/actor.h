#pragma once
#include "subiect_vizual.h"
#include <memory>
#include <string>


// adauga directia privirii, influenteaza scor compozitional
// un actor care priveste spre interiorul cadrului (nose room) e mai bine
class Actor : public SubiectVizual {
    std::string directiePrivire; // "stanga" /  "dreapta" / "camera"

    void afiseazaDetalii(std::ostream &os) const override;

public:
    explicit Actor(const std::string &denumire_,
                   const Punct &colt_,
                   double latime_,
                   double inaltime_,
                   int importanta_,
                   const std::string &directie_);

    std::unique_ptr<SubiectVizual> clone() const override;

    double contributieCompozitionala(double W, double H) const override;

    std::string sfatCompozitional() const override;

    // getter folosit in main pentru dynamic_cast cu sens
    const std::string &getDirectiePrivire() const;
};
