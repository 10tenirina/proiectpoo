#pragma once

class SubiectVizual;
class Cadru;

// Observer pattern: interfata pentru obiectele care vor sa fie notificate
// cand un Cadru este modificat (subiect adaugat, etc.).
//
// Cadrul nu cunoaste tipurile concrete de observatori. Astfel, un Logger si
// un MonitorCompozitie pot reactiona la acelasi eveniment in moduri complet
// diferite, fara ca Cadrul sa-i cunoasca - decuplare clasica Observer.
//
// Lifetime: observatorii sunt detinuti in afara Cadrului (de regula in main).
// Cadrul tine doar raw pointer; observatorii trebuie sa-l supravietuiasca.
// Aceasta este Varianta A din discutia de design: ownership simplu in main.
class ObservatorCadru {
public:
    virtual ~ObservatorCadru() = default;

    // Notificat dupa ce un subiect a fost adaugat la cadru.
    // Primeste o referinta la cadru (pentru context: titlu, scor curent)
    // si la subiectul tocmai adaugat.
    virtual void laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) = 0;
};
