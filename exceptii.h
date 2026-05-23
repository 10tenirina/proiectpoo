#pragma once
#include <exception>
#include <string>

// Ierarhie proprie de exceptii pentru Rule of Thirds Analyzer.
// Toate deriva din std::exception prin ExceptieRuleOfThirds.
//
// ExceptieRuleOfThirds       <- baza comuna, prinde orice eroare din proiect
//   ExceptieCadruInvalid     <- dimensiuni de cadru invalide (latime/inaltime <= 0)
//   ExceptieSubiectInvalid   <- date invalide pentru un subiect vizual
//   ExceptieScenaGoala       <- operatie imposibila pe scena/cadru fara subiecte

class ExceptieRuleOfThirds : public std::exception {
    std::string mesaj;
public:
    explicit ExceptieRuleOfThirds(const std::string& mesaj_)
        : mesaj{mesaj_} {}

    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

// Aruncat in constructorul Cadru cand latime sau inaltime sunt <= 0.
// Un cadru cu dimensiuni invalide nu poate calcula power points.
class ExceptieCadruInvalid : public ExceptieRuleOfThirds {
public:
    explicit ExceptieCadruInvalid(const std::string& detaliu)
        : ExceptieRuleOfThirds{"Cadru invalid: " + detaliu} {}
};

// Aruncat in constructorul SubiectVizual cand:
//   - importanta nu e in intervalul [1, 10]
//   - latime sau inaltime sunt negative
class ExceptieSubiectInvalid : public ExceptieRuleOfThirds {
public:
    explicit ExceptieSubiectInvalid(const std::string& detaliu)
        : ExceptieRuleOfThirds{"Subiect invalid: " + detaliu} {}
};

// Aruncat cand se cere o operatie pe un cadru sau scena fara subiecte:
//   - protagonistul() pe cadru gol
//   - cadruRecomandat() pe scena goala
class ExceptieScenaGoala : public ExceptieRuleOfThirds {
public:
    explicit ExceptieScenaGoala(const std::string& detaliu)
        : ExceptieRuleOfThirds{"Operatie imposibila: " + detaliu} {}
};