#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class StilCompozitional;

// Singleton: registru central cu cele patru stiluri cinematografice +
// starea de sesiune (numele stilului curent, contor de cadre analizate).
//
// Singleton e justificat aici: vrem un singur loc autoritativ pentru
// "ce stil foloseste aplicatia in acest moment" si pentru contoare globale
// de sesiune. Logger-ul, monitorul si meniul interogheaza acelasi singleton,
// in loc sa pasam stilul curent prin parametri peste tot.
//
// Initializare lazy thread-safe via "magic static" (C++11+). Constructorul
// inregistreaza cele patru stiluri. Setarea "cinematic" e default.
class RegistruStiluri {
    std::unordered_map<std::string, std::unique_ptr<StilCompozitional> > stiluri_;
    std::string stilCurent_;
    int numarCadreAnalizate_;

    RegistruStiluri();

    // necesar in .cpp pentru ca unique_ptr<StilCompozitional> are nevoie
    // de tip complet la momentul instantierii destructorului
    ~RegistruStiluri();

public:
    // Singleton: copierea/move-area instantei e interzisa.
    RegistruStiluri(const RegistruStiluri &) = delete;

    RegistruStiluri(RegistruStiluri &&) = delete;

    RegistruStiluri &operator=(const RegistruStiluri &) = delete;

    RegistruStiluri &operator=(RegistruStiluri &&) = delete;

    // Acces la instanta unica (magic static thread-safe).
    static RegistruStiluri &get();

    // Cauta un stil dupa nume. Arunca ExceptieRuleOfThirds daca e necunoscut.
    const StilCompozitional &getStil(const std::string &nume) const;

    // Stilul curent al sesiunii.
    const StilCompozitional &stilCurent() const;

    const std::string &numeStilCurent() const;

    // Schimba stilul curent. Arunca daca numele e necunoscut.
    void seteazaStilCurent(const std::string &nume);

    // Pointeri non-owning la toate stilurile inregistrate, pentru iteratie.
    std::vector<const StilCompozitional *> toateStilurile() const;

    void incrementaCadre();

    int numarCadreAnalizate() const;
};