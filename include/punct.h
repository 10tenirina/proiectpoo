#pragma once
#include <ostream>
#include <cmath>

// coord 2D (x, y) intr un cadru
// folosit ca origine a bounding box-ului in SubiectVizual
// si ca reprezentare a power points in calcul scor
class Punct {
    double x;
    double y;

public:
    Punct();

    explicit Punct(double x_, double y_);

    double getX() const;

    double getY() const;

    double distantaFata(const Punct &alt) const;

    bool operator==(const Punct &alt) const;

    bool operator!=(const Punct &alt) const;

    friend std::ostream &operator<<(std::ostream &os, const Punct &p);
};
