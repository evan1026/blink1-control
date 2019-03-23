#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    struct RGB {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;

        RGB(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept;
        RGB() noexcept;

        bool operator==(const RGB& other) const noexcept;
        bool operator!=(const RGB& other) const noexcept;

        friend std::ostream& operator<<(std::ostream& os, const RGB& rgb);
    };
}
