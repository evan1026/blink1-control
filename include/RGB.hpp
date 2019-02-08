#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    struct RGB {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;

        RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);
        RGB();

        bool operator==(const RGB& other) const;
        bool operator!=(const RGB& other) const;

        friend std::ostream& operator<<(std::ostream& os, const RGB& rgb);
    };
}
