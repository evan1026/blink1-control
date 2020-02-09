#pragma once

#include <cstdint>

#include "blink-lib/RGBN.hpp"

namespace blink1_control {
    namespace blink1_lib {
        struct PatternLineN {
            std::uint16_t fadeMillis;
            RGBN rgbn;

            PatternLineN() noexcept;
            PatternLineN(const RGBN& rgb, const std::uint16_t fadeMillis) noexcept;
            PatternLineN(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t n, const std::uint16_t fadeMillis) noexcept;

            [[nodiscard]] bool operator==(const PatternLineN& other) const noexcept;
            [[nodiscard]] bool operator!=(const PatternLineN& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine);
        };
    }
}
