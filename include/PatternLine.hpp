#pragma once

#include <cstdint>

#include "RGB.hpp"

namespace blink1_control {
    struct PatternLine {
        std::uint16_t fadeMillis;
        RGB rgb;

        PatternLine() noexcept ;
        PatternLine(const RGB& _rgb, const std::uint16_t _fadeMillis) noexcept ;
        PatternLine(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint16_t _fadeMillis) noexcept;

        bool operator==(const PatternLine& other) const noexcept;
        bool operator!=(const PatternLine& other) const noexcept;

        friend std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine);
    };
}
