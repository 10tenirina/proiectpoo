#pragma once
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

// cls template: statistici descriptive (min/max/medie/mediana)

// atribut dependent de T: std::vector<T> valori_
// metode care depind de T: minim(), maxim(), mediana() (intorc T sau double)

// cerinte pe T: trebuie sa fie comparable (operator<) si convertible la double
// pentru medie, tipurile aritmetice satisfac aceste cerinte automat

// instantieri in proiect: Statistici<double> (peste scoruri de compozitie)
// si Statistici<int> (peste importante de subiecte)

// def (header-only) deoarece e template - fara
// instantieri explicite, compilatorul le creaza la nevoie
template<typename T>
class Statistici {
    std::vector<T> valori_;

public:
    explicit Statistici(std::vector<T> valori)
        : valori_{std::move(valori)} {
    }

    std::size_t numar() const { return valori_.size(); }

    bool gol() const { return valori_.empty(); }

    T minim() const {
        if (valori_.empty())
            throw std::out_of_range("Statistici::minim pe colectie goala");
        return *std::min_element(valori_.begin(), valori_.end());
    }

    T maxim() const {
        if (valori_.empty())
            throw std::out_of_range("Statistici::maxim pe colectie goala");
        return *std::max_element(valori_.begin(), valori_.end());
    }

    double medie() const {
        if (valori_.empty())
            throw std::out_of_range("Statistici::medie pe colectie goala");
        const double suma = std::accumulate(valori_.begin(), valori_.end(), 0.0);
        return suma / static_cast<double>(valori_.size());
    }

    // intoarce double pentru consistenta cu medie() si pentru cazul size par
    // cu T=int (de ex {1,2} -> 1.5, nu 1)
    double mediana() const {
        if (valori_.empty())
            throw std::out_of_range("Statistici::mediana pe colectie goala");
        std::vector<T> copie = valori_;
        std::sort(copie.begin(), copie.end());
        const std::size_t n = copie.size();
        if (n % 2 == 1)
            return static_cast<double>(copie[n / 2]);
        return (static_cast<double>(copie[n / 2 - 1])
                + static_cast<double>(copie[n / 2])) / 2.0;
    }

    friend std::ostream &operator<<(std::ostream &os, const Statistici &s) {
        if (s.gol()) {
            os << "[colectie goala]";
            return os;
        }
        os << "n=" << s.numar()
                << " min=" << s.minim()
                << " max=" << s.maxim()
                << " medie=" << s.medie()
                << " mediana=" << s.mediana();
        return os;
    }
};
