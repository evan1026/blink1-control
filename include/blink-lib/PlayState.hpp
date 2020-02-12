#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    namespace blink1_lib {

        /**
         * The state of the blink1 devive playing patterns
         */
        struct PlayState {
            /**
             * Whether a pattern is currently playing
             */
            bool playing;

            /**
             * The index in the pattern where the play/loop started
             */
            std::uint8_t playStart;

            /**
             * The index in the pattern where the play/loop will end
             */
            std::uint8_t playEnd;

            /**
             * Number of repetitions left in a loop
             */
            std::uint8_t playCount;

            /**
             * The current index in the pattern
             */
            std::uint8_t playPos;

            /**
             * Default constructor
             *
             * Initializes all values to 0
             */
            PlayState() noexcept;

            /**
             * @param playing Whether a pattern is currently playing
             * @param playStart The index in the pattern where the play/loop started
             * @param playEnd The index in the pattern where the play/loop will end
             * @param playCount The number of repetitions left in the loop
             * @param playPos The current index in the pattern
             */
            PlayState(const bool playing, const std::uint8_t playStart, const std::uint8_t playEnd, const std::uint8_t playCount, const std::uint8_t playPos) noexcept;

            [[nodiscard]] bool operator==(const PlayState& other) const noexcept;
            [[nodiscard]] bool operator!=(const PlayState& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const PlayState& patternLine);
        };
    }
}
