#include <iostream>
#include <fstream>
#include <memory>
#include "scena.h"
#include "actor.h"    // necesar pentru dynamic_cast la Actor
#include "decor.h"    // necesar pentru adaugaSubiect manual

int main() {
    std::cout << "======= Rule of Thirds Analyzer =======\n\n";

    try {
        // citire si analiza scena din fisier
        Scena scena = Scena::dinFisier("assets/scena.txt");
        std::cout << scena;
        scena.afiseazaRaport();

        // raport detaliat pe cadrul recomandat:
        // apeleaza sfatCompozitional() si contributieCompozitionala()
        // prin pointer de baza pentru fiecare subiect
        scena.cadruRecomandat().raportDetaliat();

        // apeluri directe la scorMediu si cadruRecomandat
        std::cout << "Scor mediu scena \"" << scena.getTitlu() << "\": "
                  << scena.scorMediu() << "/100\n";
        std::cout << "Cadru recomandat: \""
                  << scena.cadruRecomandat().getTitlu() << "\"\n\n";

        // citim un cadru din fisier pentru dynamic_cast
        std::ifstream fCadru("assets/cadru_test.txt");
        if(!fCadru.is_open())
            throw ExceptieFisierInvalid("assets/cadru_test.txt", "nu poate fi deschis");
        Cadru cadruTest{};
        fCadru >> cadruTest;

        // dynamic_cast cu sens: protagonistul e Actor => afisam directia privirii
        const SubiectVizual& prot = cadruTest.protagonistul();
        const Actor* actorPtr = dynamic_cast<const Actor*>(&prot);
        if(actorPtr != nullptr) {
            std::cout << "Protagonistul \"" << actorPtr->getDenumire()
                      << "\" este Actor si priveste spre: "
                      << actorPtr->getDirectiePrivire() << "\n";
        }
        std::cout << "Total subiecte create: "
                  << SubiectVizual::getNumarSubiecteCreate() << "\n\n";
    }
    catch(const ExceptieFisierInvalid& e) {
        std::cout << "[Eroare fisier] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieCadruInvalid& e) {
        std::cout << "[Eroare cadru] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieSubiectInvalid& e) {
        std::cout << "[Eroare subiect] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieScenaGoala& e) {
        std::cout << "[Eroare scena] " << e.what() << "\n";
        return 1;
    }
    catch(const ExceptieRuleOfThirds& e) {
        std::cout << "[Eroare] " << e.what() << "\n";
        return 1;
    }

    // copy-and-swap: citim din fisier, copiem, construim scena manual
    std::cout << "--- Test copy-and-swap Cadru ---\n";
    try {
        std::ifstream fCopiere("assets/cadru_test.txt");
        if(!fCopiere.is_open())
            throw ExceptieFisierInvalid("assets/cadru_test.txt", "nu poate fi deschis");
        Cadru c1{};
        fCopiere >> c1;

        // adaugaSubiect: API public folosit pentru a adauga programatic un subiect
        // suplimentar in cadrul citit (simuleaza ce va face un viitor object detector)
        const double scorInainte = c1.calculeazaScorCompozitie();
        c1.adaugaSubiect(
            std::make_unique<Decor>("Fereastra_extra", Punct{1500.0, 100.0},
                                    200.0, 400.0, 5, "arhitectural"));
        std::cout << "adaugaSubiect: scor inainte=" << scorInainte
                  << " scor dupa adaugare manuala=" << c1.calculeazaScorCompozitie() << "\n";

        Cadru c2{c1};   // copy constructor: cloneaza subiectele prin clone()
        std::cout << "cc: scor original=" << c1.calculeazaScorCompozitie()
                  << " scor copie=" << c2.calculeazaScorCompozitie() << "\n";

        Cadru c3("Cadru_alt", 1280.0, 720.0);
        c3 = c1;        // operator= copy-and-swap
        std::cout << "op=: scor dupa atribuire=" << c3.calculeazaScorCompozitie() << "\n";

        // adaugaCadru: construim o scena din cadre incarcate din fisier
        Scena scenaTest{"Scena_construita_din_fisier"};
        scenaTest.adaugaCadru(c1);
        scenaTest.adaugaCadru(c3);
        std::cout << "Scena construita manual, scor mediu: "
                  << scenaTest.scorMediu() << "/100\n";
    }
    catch(const ExceptieRuleOfThirds& e) {
        std::cout << "[Eroare test] " << e.what() << "\n";
    }

    std::cout << "\n======= Analiza finalizata. =======\n";
    return 0;
}