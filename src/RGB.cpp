#include "RGB.hpp"

namespace blink1_control {
    RGB::RGB(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b) : r(_r), g(_g), b(_b) {}
    RGB::RGB() : r(0), g(0), b(0) {}

    bool RGB::operator==(const RGB& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool RGB::operator!=(const RGB& other) const {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const RGB& rgb) {
        os << "RGB{r=" << unsigned(rgb.r) <<  ", g=" << unsigned(rgb.g) << ", b=" << unsigned(rgb.b) << "}";
        return os;
    }
}
