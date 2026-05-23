#pragma once
#include "subiect_vizual.h"
#include <memory>

// Element focal de tip obiect important (prop).
// Recuzita statica e plasata intentionat => mic bonus la scor.
// Recuzita dinamica (ex: masina in miscare) are pozitia mai putin controlata.
class Recuzita : public SubiectVizual {
    bool esteStatic_;   // true = prop fix; false = element in miscare

    void afiseazaDetalii(std::ostream& os) const override;

public:
    explicit Recuzita(const std::string& denumire_,
                      const Punct& colt_,
                      double latime_,
                      double inaltime_,
                      int importanta_,
                      bool esteStatic__);

    std::unique_ptr<SubiectVizual> clone()                              const override;
    double                         contributieCompozitionala(double W, double H) const override;
    std::string                    sfatCompozitional()                  const override;
};