#include "sursa_lumina.h"
#include <ostream>
#include <algorithm>

SursaLumina::SursaLumina(const std::string &denumire_,
                         const Punct &colt_,
                         double latime_,
                         double inaltime_,
                         int importanta_,
                         const std::string &directie_)
    : SubiectVizual{denumire_, colt_, latime_, inaltime_, importanta_},
      directie{directie_} {
}

std::unique_ptr<SubiectVizual> SursaLumina::clone() const {
    return std::make_unique<SursaLumina>(*this);
}

double SursaLumina::contributieCompozitionala(double W, double H) const {
    double scor = scorBazaCompozitie(W, H);

    // bonus corpul de iluminat e aliniat pe o linie de treime
    // devine leading element care incadreaza scena (toleranta intre
    // cea a actorului 0.05 si cea a decorului 0.15)
    if (esteAliniat(W, H, 0.12))
        scor = std::min(100.0, scor + 12.0);

    // bonus lumina directionala (laterala sau de contur) modeleaza
    // volumele si da profunzime, lumina frontala aplatizeaza imaginea
    if (directie == "laterala" || directie == "contra")
        scor = std::min(100.0, scor + 10.0);

    return scor;
}

std::string SursaLumina::sfatCompozitional() const {
    return "Plasati sursa de lumina lateral si pe o linie de treime: "
            "lumina directionala da profunzime si ghideaza privirea";
}

void SursaLumina::afiseazaDetalii(std::ostream &os) const {
    os << getDenumire()
            << " [SursaLumina | directie: " << directie << "]"
            << " | importanta: " << getImportanta() << "/10"
            << "\n    sfat: " << sfatCompozitional();
}
