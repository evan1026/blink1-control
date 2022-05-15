#include "blink-lib/RGB.hpp"

namespace blink1_control::blink1_lib {
    RGB::RGB(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b) noexcept : r(_r), g(_g), b(_b) {}

    bool RGB::operator==(const RGB& other) const noexcept {
        return r == other.r && g == other.g && b == other.b;
    }

    bool RGB::operator!=(const RGB& other) const noexcept {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const RGB& rgb) {
        os << "RGB{r=" << unsigned(rgb.r) <<  ", g=" << unsigned(rgb.g) << ", b=" << unsigned(rgb.b) << "}";
        return os;
    }
}
