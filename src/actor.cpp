#include "actor.h"
#include <ostream>
#include <algorithm>

Actor::Actor(const std::string &denumire_,
             const Punct &colt_,
             double latime_,
             double inaltime_,
             int importanta_,
             const std::string &directie_)
    : SubiectVizual{denumire_, colt_, latime_, inaltime_, importanta_},
      directiePrivire{directie_} {
}

std::unique_ptr<SubiectVizual> Actor::clone() const {
    return std::make_unique<Actor>(*this);
}

double Actor::contributieCompozitionala(double W, double H) const {
    double scor = scorBazaCompozitie(W, H);

    // bonus: actorul e aliniat pe o linie de treime
    if (esteAliniat(W, H))
        scor = std::min(100.0, scor + 15.0);

    // bonus nose room: actorul priveste spre interiorul cadrului
    // dr din stg cadrului sau stg din dr cadrului
    const double cx = getCentru().getX();
    const bool privesteSpreInterior =
            (directiePrivire == "dreapta" && cx < W / 2.0) ||
            (directiePrivire == "stanga" && cx > W / 2.0);
    if (privesteSpreInterior)
        scor = std::min(100.0, scor + 10.0);

    return scor;
}

std::string Actor::sfatCompozitional() const {
    return "Plasati personajul pe linia verticala de treime, "
            "privind spre centrul cadrului (regula nose room)";
}

const std::string &Actor::getDirectiePrivire() const {
    return directiePrivire;
}

void Actor::afiseazaDetalii(std::ostream &os) const {
    os << getDenumire()
            << " [Actor | privire: " << directiePrivire << "]"
            << " | importanta: " << getImportanta() << "/10"
            << "\n    sfat: " << sfatCompozitional();
}
