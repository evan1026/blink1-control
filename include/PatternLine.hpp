#pragma once

#include <cstdint>

#include "RGB.hpp"

namespace blink1_control {
    struct PatternLine {
        std::uint16_t fadeMillis;
        RGB rgb;

        PatternLine();
        PatternLine(RGB _rgb, std::uint16_t _fadeMillis);
        PatternLine(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint16_t _fadeMillis);

        bool operator==(const PatternLine& other) const;
        bool operator!=(const PatternLine& other) const;

        friend std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine);
    };
}
