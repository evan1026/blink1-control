#pragma once

#include <cstdint>

#include "RGB.hpp"

namespace blink1_control {
    struct PatternLine {
        std::uint16_t fadeMillis;
        RGB rgb;
    };
    struct PatternLineN {
        std::uint16_t fadeMillis;
        RGBN rgbn;
    };
}
