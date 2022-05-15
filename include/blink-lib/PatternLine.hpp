/**
 * @file PatternLine.hpp
 * @brief Header file for blink1_control::blink1_lib::PatternLine
 */

#pragma once

#include <cstdint>

#include "blink-lib/RGB.hpp"

namespace blink1_control::blink1_lib {

    /**
     * Originally a line in a blink1 pattern, but mostly just a way to keep an RGB
     * value and a fade time together in one object
     */
    struct PatternLine {
        /**
         * Fade time in milliseconds
         */
        std::uint16_t fadeMillis{0};

        /**
         * RGB value
         */
        RGB rgb;

        /**
         * Default constructor
         *
         * Initializes all values to 0
         */
        PatternLine() noexcept = default;

        /**
         * @param _rgb RGB value for this PatternLine
         * @param _fadeMillis Fade time in milliseconds
         */
        PatternLine(const RGB& _rgb, const std::uint16_t _fadeMillis) noexcept;

        /**
         * @param _r Red value for RGB
         * @param _g Green value for RGB
         * @param _b Blue value for RGB
         * @param _fadeMillis Fade time in milliseconds
         */
        PatternLine(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint16_t _fadeMillis) noexcept;

        /**
         * Equality operator
         *
         * @param other Object to compare to
         * @return true if the objects are equal, false otherwise
         */
        [[nodiscard]] bool operator==(const PatternLine& other) const noexcept;

        /**
         * Inequality operator
         *
         * @param other Object to compare to
         * @return true if the objects are not equal, false otherwise
         */
        [[nodiscard]] bool operator!=(const PatternLine& other) const noexcept;

        /**
         * Output operator
         *
         * @param os Output stream
         * @param patternLine PatternLine to output
         */
        friend std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine);
    };
}
