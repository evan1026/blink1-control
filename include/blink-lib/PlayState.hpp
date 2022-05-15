/**
 * @file PlayState.hpp
 * @brief Header for blink1_control::blink1_lib::PlayState
 */

#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control::blink1_lib {

    /**
     * The state of the blink1 devive playing patterns
     */
    struct PlayState {
        /**
         * Whether a pattern is currently playing
         */
        bool playing{false};

        /**
         * The index in the pattern where the play/loop started
         */
        std::uint8_t playStart{0};

        /**
         * The index in the pattern where the play/loop will end
         */
        std::uint8_t playEnd{0};

        /**
         * Number of repetitions left in a loop
         */
        std::uint8_t playCount{0};

        /**
         * The current index in the pattern
         */
        std::uint8_t playPos{0};

        /**
         * Default constructor
         *
         * Initializes all values to 0
         */
        PlayState() noexcept = default;

        /**
         * @param _playing Whether a pattern is currently playing
         * @param _playStart The index in the pattern where the play/loop started
         * @param _playEnd The index in the pattern where the play/loop will end
         * @param _playCount The number of repetitions left in the loop
         * @param _playPos The current index in the pattern
         */
        PlayState(const bool _playing, const std::uint8_t _playStart, const std::uint8_t _playEnd, const std::uint8_t _playCount, const std::uint8_t _playPos) noexcept;

        /**
         * Equality operator
         *
         * @param other Object to compare to
         * @return true if the objects are equal, false otherwise
         */
        [[nodiscard]] bool operator==(const PlayState& other) const noexcept;

        /**
         * Inequality operator
         *
         * @param other Object to compare to
         * @return true if the objects are not equal, false otherwise
         */
        [[nodiscard]] bool operator!=(const PlayState& other) const noexcept;

        /**
         * Output operator
         *
         * @param os Output stream
         * @param playState PlayState to output
         */
        friend std::ostream& operator<<(std::ostream& os, const PlayState& playState);
    };
}
