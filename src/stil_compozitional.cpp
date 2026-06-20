#include "stil_compozitional.h"
#include <utility>

StilCompozitional::StilCompozitional(std::string nume)
    : nume_{std::move(nume)} {
}

const std::string &StilCompozitional::numeStil() const {
    return nume_;
}

// NVI: dispatch virtual catre afiseazaDescriere() din derivata
std::ostream &operator<<(std::ostream &os, const StilCompozitional &s) {
    s.afiseazaDescriere(os);
    return os;
}