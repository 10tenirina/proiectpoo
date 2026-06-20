#pragma once
#include "observator_cadru.h"
#include <ostream>

class StilCompozitional;

// Cele doua observatoare concrete folosite in proiect.
// Sunt grupate intr-un singur fisier - acelasi reasoning ca la stiluri:
// clase scurte, strans legate, fara state complex.

// Scrie pe un ostream (de obicei std::cerr) evenimentele primite de la Cadru.
// Util pentru audit si pentru tracing in CI (CI redirecteaza stderr separat).
class LoggerCadru : public ObservatorCadru {
    std::ostream &out_;

public:
    explicit LoggerCadru(std::ostream &out);

    void laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) override;
};

// Recalculeaza live scorul de compozitie dupa fiecare modificare, folosind
// un stil dat. Avertizeaza daca scorul scade sub un prag sau daca scade brusc
// fata de starea anterioara. Acesta este prototipul overlay-ului live de la
// camera planificat pentru licenta: detectorul adauga subiecte in cadru in
// timp real, iar monitorul reactioneaza instant.
class MonitorCompozitie : public ObservatorCadru {
    std::ostream &out_;
    const StilCompozitional &stil_;
    double prag_;
    double ultimulScor_; // -1 inseamna "primul apel, nu avem cu ce compara"

public:
    explicit MonitorCompozitie(std::ostream &out,
                               const StilCompozitional &stil,
                               double prag);

    void laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) override;
};