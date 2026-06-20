#include "registru_stiluri.h"
#include "stil_compozitional.h"
#include "stiluri.h"
#include "exceptii.h"

RegistruStiluri::RegistruStiluri()
    : stilCurent_{"cinematic"}, numarCadreAnalizate_{0} {
    // inregistram cele patru stiluri la prima creare a singleton-ului
    stiluri_["cinematic"] = std::make_unique<StilCinematic>();
    stiluri_["hollywood_clasic"] = std::make_unique<StilHollywoodClasic>();
    stiluri_["wes_anderson"] = std::make_unique<StilWesAnderson>();
    stiluri_["documentar"] = std::make_unique<StilDocumentar>();
}

// definitie out-of-line necesara pentru ca unique_ptr<StilCompozitional>
// are nevoie de definitia completa a tipului in punctul de destructie
RegistruStiluri::~RegistruStiluri() = default;

RegistruStiluri &RegistruStiluri::get() {
    // magic static: thread-safe initialization la primul apel
    static RegistruStiluri instanta;
    return instanta;
}

const StilCompozitional &RegistruStiluri::getStil(const std::string &nume) const {
    const auto it = stiluri_.find(nume);
    if (it == stiluri_.end())
        throw ExceptieRuleOfThirds("stil necunoscut: \"" + nume + "\"");
    return *it->second;
}

const StilCompozitional &RegistruStiluri::stilCurent() const {
    return getStil(stilCurent_);
}

const std::string &RegistruStiluri::numeStilCurent() const {
    return stilCurent_;
}

void RegistruStiluri::seteazaStilCurent(const std::string &nume) {
    // verificam ca exista (arunca daca nu)
    (void) getStil(nume);
    stilCurent_ = nume;
}

std::vector<const StilCompozitional *> RegistruStiluri::toateStilurile() const {
    std::vector<const StilCompozitional *> rezultat;
    rezultat.reserve(stiluri_.size());
    for (const auto &p: stiluri_)
        rezultat.push_back(p.second.get());
    return rezultat;
}

void RegistruStiluri::incrementaCadre() {
    ++numarCadreAnalizate_;
}

int RegistruStiluri::numarCadreAnalizate() const {
    return numarCadreAnalizate_;
}
