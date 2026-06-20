#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class StilCompozitional;

// Singleton: registru central cu cele patru stiluri cinematografice +
// starea de sesiune (numele stilului curent, contor de cadre analizate)

// Logger-ul, monitorul si meniul interogheaza acelasi singleton
// in loc sa pasam stilul curent prin parametri peste tot
//
//init lazy thread-safe via "magic static", constructorul
// inregistreaza cele patru stiluri, setarea "cinematic" e default
class RegistruStiluri {
    std::unordered_map<std::string, std::unique_ptr<StilCompozitional> > stiluri_;
    std::string stilCurent_;
    int numarCadreAnalizate_;

    RegistruStiluri();

    // necesar in .cpp pentru ca unique_ptr<StilCompozitional> are nevoie
    // de tip complet la momentul instantierii destructorului
    ~RegistruStiluri();

public:
    // Singleton: copierea/move-area instantei e interzisa
    RegistruStiluri(const RegistruStiluri &) = delete;

    RegistruStiluri(RegistruStiluri &&) = delete;

    RegistruStiluri &operator=(const RegistruStiluri &) = delete;

    RegistruStiluri &operator=(RegistruStiluri &&) = delete;

    // acces la instanta unica (magic static thread-safe)
    static RegistruStiluri &get();

    // cauta un stil dupa nume, arunca ExceptieRuleOfThirds daca e necunoscut
    const StilCompozitional &getStil(const std::string &nume) const;

    // stil curr al sesiunii
    const StilCompozitional &stilCurent() const;

    const std::string &numeStilCurent() const;

    // schimba stil curr, arunca daca numele e necunoscut
    void seteazaStilCurent(const std::string &nume);

    // [ointeri non-owning la toate stilurile inregistrate, pentru iteratie
    std::vector<const StilCompozitional *> toateStilurile() const;

    void incrementaCadre();

    int numarCadreAnalizate() const;
};
