#include "observatori.h"
#include "cadru.h"
#include "subiect_vizual.h"
#include "stil_compozitional.h"

// ============================================================
// LoggerCadru
// ============================================================

LoggerCadru::LoggerCadru(std::ostream &out)
    : out_{out} {
}

void LoggerCadru::laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) {
    out_ << "[LOG] cadru \"" << cadru.getTitlu()
            << "\" a primit subiect \"" << subiect.getDenumire()
            << "\" (importanta " << subiect.getImportanta() << "/10)\n";
}

// ============================================================
// MonitorCompozitie
// ============================================================

MonitorCompozitie::MonitorCompozitie(std::ostream &out,
                                     const StilCompozitional &stil,
                                     double prag)
    : out_{out}, stil_{stil}, prag_{prag}, ultimulScor_{-1.0} {
}

void MonitorCompozitie::laAdaugareSubiect(const Cadru &cadru, const SubiectVizual &subiect) {
    // calculul foloseste stilul cu care a fost configurat monitorul:
    // putem urmari acelasi cadru cu mai multi monitori, fiecare cu alt stil
    const double scor = cadru.calculeazaScorCompozitie(stil_);
    out_ << "[MONITOR/" << stil_.numeStil() << "] scor dupa adaugare \""
            << subiect.getDenumire() << "\": " << scor << "/100";

    if (ultimulScor_ >= 0.0 && scor < ultimulScor_ - 10.0)
        out_ << " - ATENTIE: scor in scadere cu peste 10 puncte";
    if (scor < prag_)
        out_ << " - ATENTIE: sub pragul " << prag_;
    out_ << "\n";

    ultimulScor_ = scor;
}
