#pragma once
#include <memory>
#include <ostream>
#include <string>

class SubiectVizual;

// forward declaration pentru enum ul definit in cadru.h, tipul subiacent
// explicit (int) ne permite sa il forward-declaram aici fara a include
// cadru.h, evitand dependenta ciclica intre cadru si stil
enum class TipCompozitie : int;

// strategy pattern: filozofia de evaluare a unui cadru

// diferiti regizori si scoli cinematografice aplica reguli diferite
//   - Wes Anderson premiaza simetria centrala pe care Hollywood-ul clasic
//     o penalizeaza ca fiind "plata"
//   - documentarul prefera autenticitatea peste compozitia "corecta"
//   - rule of thirds clasica e standardul "studio-system"

// dtilul opereaza la doua nivele, fara sa cunoasca structura interna a Cadrului:
//   - ponderaSubiect()      : ajusteaza scorul individual al unui subiect
//   - ajustarePentruTip()   : aplica bonus/penalty pe scorul agregat in functie
//                             de tipul dominant de compozitie al cadrului
//
// NVI: operator<< este non-virtual si apeleaza intern afiseazaDescriere() (virtuala pura)
class StilCompozitional {
    std::string nume_;

    virtual void afiseazaDescriere(std::ostream &os) const = 0;

protected:
    explicit StilCompozitional(std::string nume);

public:
    virtual ~StilCompozitional() = default;

    // constr virtual - polimorfism
    virtual std::unique_ptr<StilCompozitional> clone() const = 0;

    // ajusteaza scorul individual al unui subiect dupa filozofia stilului
    // primeste scorul de baza calculat de contributieCompozitionala()
    // si o referinta la subiect pentru introspectie (importanta, denumire etc)
    virtual double ponderaSubiect(const SubiectVizual &sv, double scorBaza) const = 0;

    // aplica bonus sau penalty pe scorul agregat in functie de tipul de
    // compozitie dominant al cadrului ( [0,100] )
    virtual double ajustarePentruTip(TipCompozitie tip, double scorAgregat) const = 0;

    const std::string &numeStil() const;

    friend std::ostream &operator<<(std::ostream &os, const StilCompozitional &s);
};