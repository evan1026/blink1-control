#pragma once

#include <cstdint>

#include "blink-lib/RGB.hpp"

namespace blink1_control {
    namespace blink1_lib {

        /**
         * Originally a line in a blink1 pattern, but mostly just a way to keep an RGB
         * value and a fade time together in one object
         */
        struct PatternLine {
            /**
             * Fade time in milliseconds
             */
            std::uint16_t fadeMillis;

            /**
             * RGB value
             */
            RGB rgb;

            /**
             * Default constructor
             *
             * Initializes all values to 0
             */
            PatternLine() noexcept;

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

            [[nodiscard]] bool operator==(const PatternLine& other) const noexcept;
            [[nodiscard]] bool operator!=(const PatternLine& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine);
        };
    }
}
