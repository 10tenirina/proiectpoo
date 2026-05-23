#include "subiect_vizual.h"
#include <cmath>
#include <algorithm>

// definitia variabilei statice (obligatorie intr-un singur .cpp)
int SubiectVizual::numarSubiecteCreate = 0;

SubiectVizual::SubiectVizual(const std::string& denumire_,
                              const Punct& colt_,
                              double latime_,
                              double inaltime_,
                              int importanta_)
    : denumire{denumire_},
      coltStangaSus{colt_},
      latime{latime_},
      inaltime{inaltime_},
      importanta{importanta_} {
    valideaza(latime_, inaltime_, importanta_);
    ++numarSubiecteCreate;
}

double SubiectVizual::arie() const {
    return latime * inaltime;
}

void SubiectVizual::valideaza(double latime_, double inaltime_, int importanta_) {
    if(latime_ < 0.0)
        throw ExceptieSubiectInvalid(
            "latime negativa (" + std::to_string(latime_) + ")");
    if(inaltime_ < 0.0)
        throw ExceptieSubiectInvalid(
            "inaltime negativa (" + std::to_string(inaltime_) + ")");
    if(importanta_ < 1 || importanta_ > 10)
        throw ExceptieSubiectInvalid(
            "importanta " + std::to_string(importanta_) + " nu este in [1, 10]");
}

double SubiectVizual::scorBazaCompozitie(double W, double H) const {
    const double dist = distantaFataDePowerPoint(W, H);
    const double diag = std::sqrt(W * W + H * H);
    return std::max(0.0, 100.0 - (dist / diag) * 100.0);
}

const std::string& SubiectVizual::getDenumire()   const { return denumire; }
int                SubiectVizual::getImportanta() const { return importanta; }
int                SubiectVizual::getNumarSubiecteCreate() { return numarSubiecteCreate; }

Punct SubiectVizual::getCentru() const {
    return Punct{coltStangaSus.getX() + latime   / 2.0,
                 coltStangaSus.getY() + inaltime / 2.0};
}

double SubiectVizual::distantaFataDePowerPoint(double W, double H) const {
    const Punct  centru  = getCentru();
    double       distMin = -1.0;
    const double fractii[2] = {1.0 / 3.0, 2.0 / 3.0};

    for(double fx : fractii) {
        for(double fy : fractii) {
            const Punct  pp{fx * W, fy * H};
            const double d = centru.distantaFata(pp);
            if(distMin < 0.0 || d < distMin)
                distMin = d;
        }
    }
    return distMin;
}

bool SubiectVizual::esteAliniat(double W, double H, double toleranta) const {
    const Punct  centru = getCentru();
    const double rx     = centru.getX() / W;
    const double ry     = centru.getY() / H;
    const double linii[2] = {1.0 / 3.0, 2.0 / 3.0};

    for(double l : linii) {
        if(std::abs(rx - l) <= toleranta) return true;
        if(std::abs(ry - l) <= toleranta) return true;
    }
    return false;
}

bool SubiectVizual::seSuprapuneCu(const SubiectVizual& alt) const {
    if(arie() == 0.0 || alt.arie() == 0.0) return false;

    const bool separatX =
        (coltStangaSus.getX() + latime        <= alt.coltStangaSus.getX())
     || (alt.coltStangaSus.getX() + alt.latime <= coltStangaSus.getX());

    const bool separatY =
        (coltStangaSus.getY() + inaltime        <= alt.coltStangaSus.getY())
     || (alt.coltStangaSus.getY() + alt.inaltime <= coltStangaSus.getY());

    return !separatX && !separatY;
}

// NVI: apelul virtual al afiseazaDetalii se face din operator<<
std::ostream& operator<<(std::ostream& os, const SubiectVizual& sv) {
    sv.afiseazaDetalii(os);   // dispatch virtual -> Actor / Recuzita / Decor
    return os;
}