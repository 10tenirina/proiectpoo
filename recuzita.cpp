#include "recuzita.h"
#include <ostream>
#include <algorithm>

Recuzita::Recuzita(const std::string& denumire_,
                   const Punct& colt_,
                   double latime_,
                   double inaltime_,
                   int importanta_,
                   bool esteStatic__)
    : SubiectVizual{denumire_, colt_, latime_, inaltime_, importanta_},
      esteStatic_{esteStatic__} {}

std::unique_ptr<SubiectVizual> Recuzita::clone() const {
    return std::make_unique<Recuzita>(*this);
}

double Recuzita::contributieCompozitionala(double W, double H) const {
    double scor = scorBazaCompozitie(W, H);

    // recuzita statica e plasata intentionat de regizor => mic bonus
    if(esteStatic_)
        scor = std::min(100.0, scor + 5.0);

    return scor;
}

std::string Recuzita::sfatCompozitional() const {
    return "Obiectul focal merge pe un power point, nu in centrul cadrului";
}

void Recuzita::afiseazaDetalii(std::ostream& os) const {
    os << getDenumire()
       << " [Recuzita | " << (esteStatic_ ? "statica" : "dinamica") << "]"
       << " | importanta: " << getImportanta() << "/10"
       << "\n    sfat: " << sfatCompozitional();
}