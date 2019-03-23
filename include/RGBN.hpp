#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    struct RGBN {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t n;

        RGBN(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t n);
        RGBN();

        bool operator==(const RGBN& other) const;
        bool operator!=(const RGBN& other) const;

        friend std::ostream& operator<<(std::ostream& os, const RGBN& rgb);
    };
}
