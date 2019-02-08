#pragma once

#include <cstdint>

#include "RGBN.hpp"

namespace blink1_control {
    struct PatternLineN {
        std::uint16_t fadeMillis;
        RGBN rgbn;

        PatternLineN();
        PatternLineN(RGBN rgb, std::uint16_t fadeMillis);
        PatternLineN(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t n, std::uint16_t fadeMillis);

        bool operator==(const PatternLineN& other) const;
        bool operator!=(const PatternLineN& other) const;

        friend std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine);
    };
}
