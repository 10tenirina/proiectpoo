#pragma once
#include <exception>
#include <string>

// ierarhie proprie de exceptii pentru Rule of Thirds Analyzer
// toate deriva din std::exception prin ExceptieRuleOfThirds (baza comuna)
//   ExceptieFisierInvalid, fisier inexistent sau corupt
//   ExceptieCadruInvalid, dimensiuni cadru invalide
//   ExceptieSubiectInvalid, date invalide subiect vizual
//   ExceptieScenaGoala, operatie imposibila pe scena/cadru goale

class ExceptieRuleOfThirds : public std::exception {
    std::string mesaj;
public:
    explicit ExceptieRuleOfThirds(const std::string &mesaj_)
        : mesaj{mesaj_} {
    }

    const char *what() const noexcept override { return mesaj.c_str(); }
};

// aruncat in dinFisier() - fisier inexistent sau err la citire
// primeste separat numele fisierului si motivul pentru mesaj clar
class ExceptieFisierInvalid : public ExceptieRuleOfThirds {
public:
    explicit ExceptieFisierInvalid(const std::string &numeFisier,
                                   const std::string &motiv)
        : ExceptieRuleOfThirds{"Fisier invalid \"" + numeFisier + "\": " + motiv} {}
};

// aruncat in constructorul Cadru cand latime sau inaltime sunt <= 0
class ExceptieCadruInvalid : public ExceptieRuleOfThirds {
public:
    explicit ExceptieCadruInvalid(const std::string &detaliu)
        : ExceptieRuleOfThirds{"Cadru invalid: " + detaliu} {}
};

// aruncat in constructorul SubiectVizual cand
//   - imp nu e in [1, 10]
//   - latime sau inaltime sunt negative
class ExceptieSubiectInvalid : public ExceptieRuleOfThirds {
public:
    explicit ExceptieSubiectInvalid(const std::string &detaliu)
        : ExceptieRuleOfThirds{"Subiect invalid: " + detaliu} {}
};

// aruncat cand se cere protagonistul() sau cadruRecomandat()
// pe un cadru/scena fara subiecte/cadre
class ExceptieScenaGoala : public ExceptieRuleOfThirds {
public:
    explicit ExceptieScenaGoala(const std::string &detaliu)
        : ExceptieRuleOfThirds{"Operatie imposibila: " + detaliu} {}
};