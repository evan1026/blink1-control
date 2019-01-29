#pragma once

#include <cstdint>

namespace blink1_control {
    struct RGB {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
    };

    struct RGBN {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t n;
    };
}
