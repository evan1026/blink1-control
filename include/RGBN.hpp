#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    struct RGBN {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t n;

        RGBN(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t n) noexcept;
        RGBN() noexcept;

        bool operator==(const RGBN& other) const noexcept;
        bool operator!=(const RGBN& other) const noexcept;

        friend std::ostream& operator<<(std::ostream& os, const RGBN& rgb);
    };
}
