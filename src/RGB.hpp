#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    struct RGB {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;

        RGB(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b) : r(_r), g(_g), b(_b) {}
        RGB() : r(0), g(0), b(0) {}

        friend std::ostream& operator<<(std::ostream& os, const RGB& rgb) {
            os << "RGB{r=" << unsigned(rgb.r) <<  ", g=" << unsigned(rgb.g) << ", b=" << unsigned(rgb.b) << "}";
            return os;
        }
    };

    struct RGBN {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t n;

        RGBN(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _n) : r(_r), g(_g), b(_b), n(_n) {}
        RGBN() : r(0), g(0), b(0), n(0) {}

        friend std::ostream& operator<<(std::ostream& os, const RGBN& rgb) {
            os << "RGBN{r=" << unsigned(rgb.r) <<  ", g=" << unsigned(rgb.g) << ", b=" << unsigned(rgb.b) << ", n=" << rgb.n << "}";
            return os;
        }
    };
}
