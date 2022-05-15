/**
 * @file PatternLineN.hpp
 * @brief Header file for blink1_control::blink1_lib::PatternLineN
 */

#pragma once

#include <cstdint>

#include "blink-lib/RGBN.hpp"

namespace blink1_control::blink1_lib {

    /**
     * Same as a PatternLine, but uses RGBN to also store an LED ID
     */
    struct PatternLineN {
        /**
         * Fade time in milliseconds
         */
        std::uint16_t fadeMillis{0};

        /**
         * RGBN value
         */
        RGBN rgbn;

        /**
         * Default constructor
         *
         * Initializes all values to 0
         */
        PatternLineN() noexcept = default;

        /**
         * @param _rgbn RGBN value
         * @param _fadeMillis Fade time in milliseconds
         */
        PatternLineN(const RGBN& _rgbn, const std::uint16_t _fadeMillis) noexcept;

        /**
         * @param _r Red value
         * @param _g Green value
         * @param _b Blue value
         * @param _n LED index
         * @param _fadeMillis Fade time in milliseconds
         */
        PatternLineN(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint8_t _n, const std::uint16_t _fadeMillis) noexcept;

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
