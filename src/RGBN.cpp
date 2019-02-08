#include "RGBN.hpp"

namespace blink1_control {
    RGBN::RGBN(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _n) : r(_r), g(_g), b(_b), n(_n) {}
    RGBN::RGBN() : r(0), g(0), b(0), n(0) {}

    bool RGBN::operator==(const RGBN& other) const {
        return r == other.r && g == other.g && b == other.b && n == other.n;
    }

    bool RGBN::operator!=(const RGBN& other) const {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const RGBN& rgb) {
        os << "RGBN{r=" << unsigned(rgb.r) <<  ", g=" << unsigned(rgb.g) << ", b=" << unsigned(rgb.b) << ", n=" << unsigned(rgb.n) << "}";
        return os;
    }
}
