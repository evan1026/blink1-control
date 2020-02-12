#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    namespace blink1_lib {

        /**
         * Represents a color
         */
        struct RGB {

            /**
             * Red component
             */
            std::uint8_t r;

            /**
             * Green component
             */
            std::uint8_t g;

            /**
             * Blue component
             */
            std::uint8_t b;

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
            RGB() noexcept;

            [[nodiscard]] bool operator==(const RGB& other) const noexcept;
            [[nodiscard]] bool operator!=(const RGB& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const RGB& rgb);
        };
    }
}
