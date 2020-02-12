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

            [[nodiscard]] bool operator==(const RGBN& other) const noexcept;
            [[nodiscard]] bool operator!=(const RGBN& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const RGBN& rgb);
        };
    }
}
