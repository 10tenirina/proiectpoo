#pragma once
#include "observator_cadru.h"
#include <ostream>

class StilCompozitional;

class LoggerCadru : public ObservatorCadru {
    std::ostream &out_;

public:
    explicit LoggerCadru(std::ostream &out);

    void laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) override;
};

// recalc live scorul de compozitie dupa fiecare modif folosind
// un stil dat, avertizeaza daca scorul scade sub un prag sau daca scade brusc
// fata de starea anterioara
class MonitorCompozitie : public ObservatorCadru {
    std::ostream &out_;
    const StilCompozitional &stil_;
    double prag_;
    double ultimulScor_; // -1 inseamna primul apel, nu avem cu ce compara

public:
    explicit MonitorCompozitie(std::ostream &out,
                               const StilCompozitional &stil,
                               double prag);

    void laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) override;
};
