#include <iostream>
#include <fstream>
#include <memory>
#include <cstddef>
#include <vector>
#include "scena.h"
#include "actor.h"    // necesar pentru dynamic_cast si extragePeTip<Actor>
#include "decor.h"    // necesar pentru adaugaSubiect manual
#include "recuzita.h" // necesar pentru demo Observer
#include "sursa_lumina.h"  // necesar pentru extragePeTip<SursaLumina>
#include "stil_compozitional.h"
#include "stiluri.h"
#include "observator_cadru.h"
#include "observatori.h"
#include "registru_stiluri.h"

namespace {
    void afiseazaMeniu() {
        std::cout << "\n=== Meniu interactiv (comenzi din stdin / tastatura.txt) ===\n"
                << "  1             - afiseaza raportul scenei\n"
                << "  2             - afiseaza clasamentul cadrelor\n"
                << "  3 i j         - compara cadrele i si j\n"
                << "  4 i           - analiza detaliata a cadrului i (scor pe fiecare subiect + tip + echilibru)\n"
                << "  5 i <subiect> - adauga un subiect in cadrul i (declanseaza observatorii)\n"
                << "  6 i           - evalueaza cadrul i cu toate stilurile cinematografice (Strategy)\n"
                << "  7 nume_stil   - schimba stilul global al sesiunii (Singleton)\n"
                << "  8             - afiseaza statistici peste scorurile cadrelor (Statistici<double>)\n"
                << "  0             - iesire\n"
                << "Alege optiunea: ";
    }

    // Bucla de meniu condusa de EOF: la stdin gol (tastatura.txt gol) iese imediat,
    // deci nu blocheaza niciodata CI-ul.
    // Singleton-ul RegistruStiluri e accesat direct cand e nevoie - nu mai pasam
    // un vector de stiluri ca parametru.
    void ruleazaMeniu(Scena &scena) {
        afiseazaMeniu();
        int optiune = 0;
        while (std::cin >> optiune) {
            if (optiune == 0) {
                std::cout << "Iesire din modul interactiv.\n";
                break;
            }
            try {
                switch (optiune) {
                    case 1:
                        scena.afiseazaRaport();
                        break;
                    case 2:
                        scena.afiseazaClasament();
                        break;
                    case 3: {
                        std::size_t i = 0;
                        std::size_t j = 0;
                        std::cin >> i >> j;
                        scena.comparaCadre(i, j);
                        break;
                    }
                    case 4: {
                        std::size_t i = 0;
                        std::cin >> i;
                        scena.afiseazaAnalizaCadru(i);
                        break;
                    }
                    case 5: {
                        std::size_t i = 0;
                        std::cin >> i;
                        scena.adaugaSubiectLaCadru(i, creeazaSubiectDinStream(std::cin));
                        std::cout << "Subiect adaugat in cadrul " << i << ".\n";
                        break;
                    }
                    case 6: {
                        std::size_t i = 0;
                        std::cin >> i;
                        scena.evalueazaCadruCuStiluri(i, RegistruStiluri::get().toateStilurile());
                        // contor sesiune: incrementam la fiecare evaluare comandata
                        RegistruStiluri::get().incrementaCadre();
                        break;
                    }
                    case 7: {
                        std::string nume;
                        std::cin >> nume;
                        auto &r = RegistruStiluri::get();
                        const std::string vechi = r.numeStilCurent();
                        r.seteazaStilCurent(nume);
                        std::cout << "Stil global: \"" << vechi << "\" -> \""
                                << r.numeStilCurent() << "\"\n";
                        break;
                    }
                    case 8: {
                        const auto stats = scena.statisticiScoruri();
                        std::cout << "Statistici scoruri scena \"" << scena.getTitlu()
                                << "\": " << stats << "\n";
                        break;
                    }
                    default:
                        std::cout << "Optiune invalida: " << optiune << "\n";
                        break;
                }
            } catch (const ExceptieRuleOfThirds &e) {
                std::cout << "[Eroare comanda] " << e.what() << "\n";
            } catch (const std::out_of_range &e) {
                std::cout << "[Eroare statistici] " << e.what() << "\n";
            }
            std::cout << "Alege optiunea: ";
        }
    }
} // namespace

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

        // clasament cadre dupa scor (std::sort)
        scena.afiseazaClasament();

        // Strategy: aceleasi cadre vazute prin filozofii cinematografice diferite.
        // Un cadru centrat e "slab" pentru Hollywood dar excelent pentru Wes Anderson.
        // Stilurile sunt centralizate in Singleton-ul RegistruStiluri.
        std::cout << "--- Strategy: scoruri pe fiecare cadru cu fiecare stil ---\n";
        const auto stiluriDemo = RegistruStiluri::get().toateStilurile();
        for (std::size_t i = 0; i < stiluriDemo.size(); ++i) {
            // demonstram si polimorfismul clone() prin pointer de baza
            auto copie = stiluriDemo[i]->clone();
            std::cout << "  Stil disponibil " << (i + 1) << ": " << *copie << "\n";
        }
        std::cout << "\n";
        scena.evalueazaCadruCuStiluri(0, stiluriDemo);
        scena.evalueazaCadruCuStiluri(1, stiluriDemo);

        // analiza de compozitie pe cadrul recomandat: tip + echilibru vizual
        const Cadru &recomandat = scena.cadruRecomandat();
        std::cout << "Tip compozitie cadru recomandat: "
                  << descriereTipCompozitie(recomandat.tipCompozitie()) << "\n";
        std::cout << "Echilibru vizual: " << recomandat.analizeazaEchilibru() << "\n\n";

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
        c3 = c1; // operator= copy-and-swap
        std::cout << "op=: scor dupa atribuire=" << c3.calculeazaScorCompozitie() << "\n";

        // construim un al doilea cadru, deliberat centrat (compozitie slaba),
        // si il comparam side-by-side cu cadrul de mai sus
        Cadru c4{"Cadru_centrat", 1920.0, 1080.0};
        c4.adaugaSubiect(std::make_unique<Actor>(
            "Protagonist_centrat", Punct{890.0, 430.0}, 140.0, 220.0, 9, "camera"));
        c4.adaugaSubiect(std::make_unique<Decor>(
            "Perete", Punct{800.0, 400.0}, 300.0, 500.0, 5, "arhitectural"));
        c1.comparaCu(c4);

        // adaugaCadru: construim o scena din cadre incarcate din fisier
        Scena scenaTest{"Scena_construita_din_fisier"};
        scenaTest.adaugaCadru(c1);
        scenaTest.adaugaCadru(c3);
        std::cout << "Scena construita manual, scor mediu: "
                << scenaTest.scorMediu() << "/100\n";
    } catch (const ExceptieRuleOfThirds &e) {
        std::cout << "[Eroare test] " << e.what() << "\n";
    }

    // Observer: notificari live la modificarea cadrului.
    // Logger-ul scrie pe stderr (separat de output principal),
    // monitorul recalculeaza scorul si avertizeaza daca scade sub prag.
    // Monitorul foloseste stilul curent din Singleton (cinematic, default).
    std::cout << "\n--- Observer: notificari live la adaugare subiect ---\n";
    try {
        LoggerCadru loggerDemo{std::cerr};
        MonitorCompozitie monitorDemo{std::cout, RegistruStiluri::get().stilCurent(), 50.0};

        Cadru cObs{"Cadru_observat", 1920.0, 1080.0};
        cObs.adaugaObservator(&loggerDemo);
        cObs.adaugaObservator(&monitorDemo);

        // primul subiect: scor pleaca de la 0 (cadru gol) la ceva rezonabil
        cObs.adaugaSubiect(std::make_unique<Actor>(
            "Heroina", Punct{600.0, 280.0}, 140.0, 220.0, 9, "dreapta"));
        // al doilea: contribuie cu sfat compozitional bun, monitorul vede crestere
        cObs.adaugaSubiect(std::make_unique<Decor>(
            "Fereastra_treime", Punct{200.0, 100.0}, 300.0, 500.0, 6, "arhitectural"));
        // al treilea: deliberat slab plasat, ca sa vedem avertismentul monitorului
        cObs.adaugaSubiect(std::make_unique<Recuzita>(
            "Obiect_aiurea", Punct{960.0, 540.0}, 50.0, 50.0, 2, true));

        // detasam observatorii: urmatoarea adaugare nu mai produce notificari
        cObs.eliminaObservatori();
        std::cout << "  (observatorii detasati)\n";
        cObs.adaugaSubiect(std::make_unique<Recuzita>(
            "Obiect_tacut", Punct{700.0, 400.0}, 30.0, 30.0, 1, true));
        std::cout << "  scor final dupa detasare: "
                << cObs.calculeazaScorCompozitie() << "/100\n";
    } catch (const ExceptieRuleOfThirds &e) {
        std::cout << "[Eroare demo observer] " << e.what() << "\n";
    }

    // Singleton: registru central de stiluri + stare de sesiune.
    std::cout << "\n--- Singleton: RegistruStiluri ---\n";
    {
        auto &reg = RegistruStiluri::get();
        std::cout << "Stil global curent: \"" << reg.numeStilCurent() << "\"\n";
        std::cout << "  descriere: " << reg.stilCurent() << "\n";
        reg.seteazaStilCurent("wes_anderson");
        std::cout << "Schimbat la: \"" << reg.numeStilCurent() << "\"\n";
        reg.seteazaStilCurent("cinematic");
        std::cout << "Revenit la: \"" << reg.numeStilCurent() << "\"\n";
        try {
            reg.seteazaStilCurent("stil_inexistent");
        } catch (const ExceptieRuleOfThirds &e) {
            std::cout << "Verificare validare: " << e.what() << "\n";
        }
        reg.incrementaCadre();
        reg.incrementaCadre();
        reg.incrementaCadre();
        std::cout << "Cadre analizate in sesiune: "
                << reg.numarCadreAnalizate() << "\n";
    }

    // Clase si functii template: Statistici<T> si Cadru::extragePeTip<T>.
    std::cout << "\n--- Statistici<double>: scoruri scena ---\n";
    try {
        Scena scenaStats = Scena::dinFisier("assets/scena.txt");
        const auto statsScoruri = scenaStats.statisticiScoruri();
        std::cout << "  numar   = " << statsScoruri.numar() << "\n"
                << "  minim   = " << statsScoruri.minim() << "/100\n"
                << "  maxim   = " << statsScoruri.maxim() << "/100\n"
                << "  medie   = " << statsScoruri.medie() << "/100\n"
                << "  mediana = " << statsScoruri.mediana() << "/100\n"
                << "  rezumat: " << statsScoruri << "\n";

        std::cout << "\n--- Statistici<int>: importante in cadrul recomandat ---\n";
        const Cadru &cadruStats = scenaStats.cadruRecomandat();
        const auto statsImp = cadruStats.statisticiImportanta();
        std::cout << "  numar   = " << statsImp.numar() << "\n"
                << "  minim   = " << statsImp.minim() << "\n"
                << "  maxim   = " << statsImp.maxim() << "\n"
                << "  medie   = " << statsImp.medie() << "\n"
                << "  mediana = " << statsImp.mediana() << "\n"
                << "  rezumat: " << statsImp << "\n";

        std::cout << "\n--- Functie template Cadru::extragePeTip<T> ---\n";
        // Instantiere 1: extragem doar actorii (avem nevoie de directiaPrivirii)
        const auto actoriRec = cadruStats.extragePeTip<Actor>();
        std::cout << "Actori in cadrul \"" << cadruStats.getTitlu()
                << "\" (" << actoriRec.size() << "):\n";
        for (const Actor *a: actoriRec)
            std::cout << "  " << a->getDenumire()
                    << " (priveste \"" << a->getDirectiePrivire() << "\")\n";
        // Instantiere 2: extragem sursele de lumina (am vrea directia, dar
        // SursaLumina nu expune getter pentru ea -- arătăm doar denumirile)
        const auto surseRec = cadruStats.extragePeTip<SursaLumina>();
        std::cout << "Surse de lumina in acelasi cadru ("
                << surseRec.size() << "):\n";
        for (const SursaLumina *s: surseRec)
            std::cout << "  " << s->getDenumire() << "\n";
    } catch (const ExceptieRuleOfThirds &e) {
        std::cout << "[Eroare demo templates] " << e.what() << "\n";
    } catch (const std::out_of_range &e) {
        std::cout << "[Eroare statistici] " << e.what() << "\n";
    }

    // Mod interactiv: comenzi din stdin (redirectate din tastatura.txt in CI).
    // Meniul acceseaza Singleton-ul direct cand are nevoie de stiluri,
    // deci nu mai pasam un vector ca parametru.
    try {
        // observatorii referă Singleton-ul prin stilCurent() la momentul
        // construirii. Schimbarile ulterioare ale stilului curent (optiune 7)
        // nu afecteaza monitorul deja instalat - el ramane pe stilul initial.
        LoggerCadru loggerMeniu{std::cerr};
        MonitorCompozitie monitorMeniu{
            std::cout, RegistruStiluri::get().stilCurent(), 50.0};

        Scena scenaInteractiva = Scena::dinFisier("assets/scena.txt");
        // Observer: orice optiune 5 din meniu va declansa notificari prin
        // logger si monitor pe cadrul vizat.
        scenaInteractiva.adaugaObservatorLaToateCadrele(&loggerMeniu);
        scenaInteractiva.adaugaObservatorLaToateCadrele(&monitorMeniu);

        ruleazaMeniu(scenaInteractiva);
    }
    catch (const ExceptieRuleOfThirds &e) {
        std::cout << "[Eroare] " << e.what() << "\n";
    }

    std::cout << "\nTotal cadre analizate prin Singleton: "
            << RegistruStiluri::get().numarCadreAnalizate() << "\n";

    std::cout << "\n======= Analiza finalizata. =======\n";
    return 0;
}