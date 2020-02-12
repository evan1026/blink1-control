#pragma once

#include <cstdint>

#include "blink-lib/RGBN.hpp"

namespace blink1_control {
    namespace blink1_lib {

        /**
         * Same as a PatternLine, but uses RGBN to also store an LED ID
         */
        struct PatternLineN {
            /**
             * Fade time in milliseconds
             */
            std::uint16_t fadeMillis;

            /**
             * RGBN value
             */
            RGBN rgbn;

            /**
             * Default constructor
             *
             * Initializes all values to 0
             */
            PatternLineN() noexcept;

            /**
             * @param rgb RGBN value
             * @param fadeMillis Fade time in milliseconds
             */
            PatternLineN(const RGBN& rgb, const std::uint16_t fadeMillis) noexcept;

            /**
             * @param r Red value
             * @param g Green value
             * @param b Blue value
             * @param n LED index
             * @param fadeMillis Fade time in milliseconds
             */
            PatternLineN(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t n, const std::uint16_t fadeMillis) noexcept;

            [[nodiscard]] bool operator==(const PatternLineN& other) const noexcept;
            [[nodiscard]] bool operator!=(const PatternLineN& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine);
        };
    }
}
