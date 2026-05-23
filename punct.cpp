#include "punct.h"

Punct::Punct() : x{0.0}, y{0.0} {}

Punct::Punct(double x_, double y_) : x{x_}, y{y_} {}

double Punct::getX() const { return x; }
double Punct::getY() const { return y; }

double Punct::distantaFata(const Punct& alt) const {
    const double dx = x - alt.x;
    const double dy = y - alt.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Punct::operator==(const Punct& alt) const {
    return x == alt.x && y == alt.y;
}

bool Punct::operator!=(const Punct& alt) const {
    return !(*this == alt);
}

std::ostream& operator<<(std::ostream& os, const Punct& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}