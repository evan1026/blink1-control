/**
 * @file RGBN.hpp
 * @brief Header file for blink1_control::blink1_lib::RGBN
 */

#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    namespace blink1_lib {

        /**
         * A datatype that combines an RGB value with an LED index
         */
        struct RGBN {
            /**
             * Red value
             */
            std::uint8_t r;

            /**
             * Green value
             */
            std::uint8_t g;

            /**
             * Blue value
             */
            std::uint8_t b;

            /**
             * LED index
             */
            std::uint8_t n;

            /**
             * @param r Red value
             * @param g Green value
             * @param b Blue value
             * @param n LED index
             */
            RGBN(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t n) noexcept;

            /**
             * Default constructor
             *
             * Initializes all values to 0
             */
            RGBN() noexcept;

            /**
             * Equality operator
             *
             * @param other Object to compare to
             * @return true if the objects are equal, false otherwise
             */
            [[nodiscard]] bool operator==(const RGBN& other) const noexcept;

            /**
             * Inequality operator
             *
             * @param other Object to compare to
             * @return true if the objects are not equal, false otherwise
             */
            [[nodiscard]] bool operator!=(const RGBN& other) const noexcept;

            /**
             * Output operator
             *
             * @param os Output stream
             * @param rgb RGBN object to output
             */
            friend std::ostream& operator<<(std::ostream& os, const RGBN& rgb);
        };
    }
}
