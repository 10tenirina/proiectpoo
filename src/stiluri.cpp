#include "stiluri.h"
#include "cadru.h"   // pentru valorile concrete ale enum-ului TipCompozitie
#include <algorithm>

// ============================================================
// StilCinematic - implicit, neutru
// ============================================================

StilCinematic::StilCinematic()
    : StilCompozitional{"cinematic"} {
}

std::unique_ptr<StilCompozitional> StilCinematic::clone() const {
    return std::make_unique<StilCinematic>(*this);
}

double StilCinematic::ponderaSubiect(const SubiectVizual & /*sv*/, double scorBaza) const {
    return scorBaza;
}

double StilCinematic::ajustarePentruTip(TipCompozitie /*tip*/, double scorAgregat) const {
    return scorAgregat;
}

void StilCinematic::afiseazaDescriere(std::ostream &os) const {
    os << "[" << numeStil() << "] stil neutru, rule of thirds clasica fara ponderi";
}

// ============================================================
// StilHollywoodClasic - rule of thirds stricta, centrarea penalizata
// ============================================================

StilHollywoodClasic::StilHollywoodClasic()
    : StilCompozitional{"hollywood_clasic"} {
}

std::unique_ptr<StilCompozitional> StilHollywoodClasic::clone() const {
    return std::make_unique<StilHollywoodClasic>(*this);
}

double StilHollywoodClasic::ponderaSubiect(const SubiectVizual & /*sv*/, double scorBaza) const {
    return scorBaza;
}

double StilHollywoodClasic::ajustarePentruTip(TipCompozitie tip, double scorAgregat) const {
    switch (tip) {
        case TipCompozitie::ReguliTreimilor:
            return std::min(100.0, scorAgregat + 5.0);
        case TipCompozitie::Centrata:
            return std::max(0.0, scorAgregat - 15.0);
        case TipCompozitie::Simetrica:
        case TipCompozitie::Echilibrata:
            return scorAgregat;
    }
    return scorAgregat;
}

void StilHollywoodClasic::afiseazaDescriere(std::ostream &os) const {
    os << "[" << numeStil() << "] rule of thirds stricta; "
            << "compozitia centrata e penalizata (cinema studio-system)";
}

// ============================================================
// StilWesAnderson - simetrie centrala perfecta
// ============================================================

StilWesAnderson::StilWesAnderson()
    : StilCompozitional{"wes_anderson"} {
}

std::unique_ptr<StilCompozitional> StilWesAnderson::clone() const {
    return std::make_unique<StilWesAnderson>(*this);
}

double StilWesAnderson::ponderaSubiect(const SubiectVizual & /*sv*/, double scorBaza) const {
    return scorBaza;
}

double StilWesAnderson::ajustarePentruTip(TipCompozitie tip, double scorAgregat) const {
    switch (tip) {
        case TipCompozitie::Centrata:
            return std::min(100.0, scorAgregat + 20.0);
        case TipCompozitie::Simetrica:
            return std::min(100.0, scorAgregat + 15.0);
        case TipCompozitie::ReguliTreimilor:
            return std::max(0.0, scorAgregat - 10.0);
        case TipCompozitie::Echilibrata:
            return std::max(0.0, scorAgregat - 5.0);
    }
    return scorAgregat;
}

void StilWesAnderson::afiseazaDescriere(std::ostream &os) const {
    os << "[" << numeStil() << "] simetrie centrala perfecta; "
            << "rule of thirds este considerata plata";
}

// ============================================================
// StilDocumentar - cinema verite, autenticitatea primeaza
// ============================================================

StilDocumentar::StilDocumentar()
    : StilCompozitional{"documentar"} {
}

std::unique_ptr<StilCompozitional> StilDocumentar::clone() const {
    return std::make_unique<StilDocumentar>(*this);
}

double StilDocumentar::ponderaSubiect(const SubiectVizual & /*sv*/, double scorBaza) const {
    // aplatizam scorurile spre 50: documentarul nu critica autenticitatea
    return 0.6 * scorBaza + 0.4 * 50.0;
}

double StilDocumentar::ajustarePentruTip(TipCompozitie /*tip*/, double scorAgregat) const {
    // bonus uniform: orice incadrare e acceptabila in documentar
    return std::min(100.0, scorAgregat + 5.0);
}

void StilDocumentar::afiseazaDescriere(std::ostream &os) const {
    os << "[" << numeStil() << "] cinema verite; "
       << "toleranta mare la abateri, naturalismul primeaza";
}