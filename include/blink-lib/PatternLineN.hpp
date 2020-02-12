/**
 * @file PatternLineN.hpp
 * @brief Header file for blink1_control::blink1_lib::PatternLineN
 */

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

            /**
             * Equality operator
             *
             * @param other Object to compare to
             * @return true if the objects are equal, false otherwise
             */
            [[nodiscard]] bool operator==(const PatternLineN& other) const noexcept;

            /**
             * Inequality operator
             *
             * @param other Object to compare to
             * @return true if the objects are not equal, false otherwise
             */
            [[nodiscard]] bool operator!=(const PatternLineN& other) const noexcept;

            /**
             * Output operator
             *
             * @param os Output stream
             * @param patternLine PatternLineN to output
             */
            friend std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine);
        };
    }
}
