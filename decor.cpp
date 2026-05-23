#include "decor.h"
#include <ostream>
#include <algorithm>

Decor::Decor(const std::string& denumire_,
             const Punct& colt_,
             double latime_,
             double inaltime_,
             int importanta_,
             const std::string& tipDecor_)
    : SubiectVizual{denumire_, colt_, latime_, inaltime_, importanta_},
      tipDecor{tipDecor_} {}

std::unique_ptr<SubiectVizual> Decor::clone() const {
    return std::make_unique<Decor>(*this);
}

double Decor::contributieCompozitionala(double W, double H) const {
    double scor = scorBazaCompozitie(W, H);

    // bonus: decorul taie o linie de treime (toleranta mai mare decat la actor)
    // => creaza un leading line care ghideaza privirea spre subiect
    if(esteAliniat(W, H, 0.15))
        scor = std::min(100.0, scor + 10.0);

    return scor;
}

std::string Decor::sfatCompozitional() const {
    return "Elementul de decor ar trebui sa creeze o rama naturala "
           "sau sa ghideze privirea spre subiectul principal";
}

void Decor::afiseazaDetalii(std::ostream& os) const {
    os << getDenumire()
       << " [Decor " << tipDecor << "]"
       << " | importanta: " << getImportanta() << "/10"
       << "\n    sfat: " << sfatCompozitional();
}