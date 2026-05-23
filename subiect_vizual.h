#pragma once
#include <string>
#include <memory>
#include <ostream>
#include "punct.h"
#include "exceptii.h"

// Clasa abstracta de baza pentru orice element focal dintr-un cadru.
// Defineste interfata comuna (bounding box, importanta, calcule compozitionale).
// Nu poate fi instantiata direct - deriva Actor, Recuzita, Decor.
//
// NVI (Non-Virtual Interface):
//   operator<< este non-virtual si public, dar intern apeleaza
//   afiseazaDetalii() care este virtuala pura si privata.
//   Astfel fiecare clasa derivata controleaza afisarea proprie,
//   fara ca operatorul sa fie virtual.
class SubiectVizual {
    std::string denumire;
    Punct       coltStangaSus;
    double      latime;
    double      inaltime;
    int         importanta;   // [1, 10]

    // contor static: cate subiecte au fost create in total
    static int numarSubiecteCreate;

    double arie() const;
    static void valideaza(double latime_, double inaltime_, int importanta_);

    // NVI: privata, pura, implementata de fiecare derivata
    virtual void afiseazaDetalii(std::ostream& os) const = 0;

protected:
    // accesibil doar din derivate
    explicit SubiectVizual(const std::string& denumire_,
                           const Punct& colt_,
                           double latime_,
                           double inaltime_,
                           int importanta_);

    // helper pentru contributieCompozitionala in derivate:
    // calculeaza scorul de baza bazat pe distanta fata de cel mai apropiat power point
    // singurul detaliu de implementare expus derivatelor - strict minimul necesar
    double scorBazaCompozitie(double W, double H) const;

public:
    virtual ~SubiectVizual() = default;

    // constructor virtual - necesar pentru copierea corecta prin pointer de baza
    // fiecare derivata returneaza o copie a ei insasi
    virtual std::unique_ptr<SubiectVizual> clone() const = 0;

    // functie virtuala specifica temei:
    // calculeaza contributia individuala la scorul de compozitie (0-100)
    // fiecare tip de subiect aplica bonusuri/penalizari diferite
    virtual double contributieCompozitionala(double W, double H) const = 0;

    // functie virtuala specifica temei:
    // returneaza o recomandare textuala de compozitie specifica tipului
    virtual std::string sfatCompozitional() const = 0;

    // metode comune (non-virtuale) - logica geometrica aceeasi pentru toti
    const std::string& getDenumire()   const;
    int                getImportanta() const;
    Punct              getCentru()     const;

    double distantaFataDePowerPoint(double W, double H) const;
    bool   esteAliniat(double W, double H, double toleranta = 0.05) const;
    bool   seSuprapuneCu(const SubiectVizual& alt) const;

    // static: numar total de subiecte create de la lansarea programului
    static int getNumarSubiecteCreate();

    // NVI: non-virtual, apeleaza intern afiseazaDetalii() (virtual)
    friend std::ostream& operator<<(std::ostream& os, const SubiectVizual& sv);
};