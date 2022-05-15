/**
 * @file RGB.hpp
 * @brief Header file for blink1_control::blink1_lib::RGB
 */

#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control::blink1_lib {

    /**
     * Represents a color
     */
    struct RGB {

        /**
         * Red component
         */
        std::uint8_t r{0};

        /**
         * Green component
         */
        std::uint8_t g{0};

        /**
         * Blue component
         */
        std::uint8_t b{0};

        /**
         * @param r Red value
         * @param g Green value
         * @param b Blue value
         */
        RGB(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept;

        /**
         * Default constructor
         *
         * Intializes all values to 0
         */
        RGB() noexcept = default;

        /**
         * Equality operator
         *
         * @param other Object to compare to
         * @return true if the objects are equal, false otherwise
         */
        [[nodiscard]] bool operator==(const RGB& other) const noexcept;

        /**
         * Inequality operator
         *
         * @param other Object to compare to
         * @return true if the objects are not equal, false otherwise
         */
        [[nodiscard]] bool operator!=(const RGB& other) const noexcept;

        /**
         * Output operator
         *
         * @param os Output stream
         * @param rgb RGB object to output
         */
        friend std::ostream& operator<<(std::ostream& os, const RGB& rgb);
    };
}
