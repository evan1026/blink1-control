#pragma once

#include <cstdint>

#include "RGB.hpp"

namespace blink1_control {
    struct PatternLine {
        std::uint16_t fadeMillis;
        RGB rgb;

        PatternLine();
        PatternLine(const RGB& _rgb, const std::uint16_t _fadeMillis);
        PatternLine(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint16_t _fadeMillis);

        bool operator==(const PatternLine& other) const;
        bool operator!=(const PatternLine& other) const;

        friend std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine);
    };
}
