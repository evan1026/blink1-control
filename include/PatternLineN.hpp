#pragma once

#include <cstdint>

#include "RGBN.hpp"

namespace blink1_control {
    struct PatternLineN {
        std::uint16_t fadeMillis;
        RGBN rgbn;

        PatternLineN();
        PatternLineN(const RGBN& rgb, const std::uint16_t fadeMillis);
        PatternLineN(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t n, const std::uint16_t fadeMillis);

        bool operator==(const PatternLineN& other) const;
        bool operator!=(const PatternLineN& other) const;

        friend std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine);
    };
}
