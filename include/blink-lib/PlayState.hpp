#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    namespace blink1_lib {
        struct PlayState {
            bool playing;
            std::uint8_t playStart;
            std::uint8_t playEnd;
            std::uint8_t playCount;
            std::uint8_t playPos;

            PlayState() noexcept;
            PlayState(const bool playing, const std::uint8_t playStart, const std::uint8_t playEnd, const std::uint8_t playCount, const std::uint8_t playPos) noexcept;

            [[nodiscard]] bool operator==(const PlayState& other) const noexcept;
            [[nodiscard]] bool operator!=(const PlayState& other) const noexcept;

            friend std::ostream& operator<<(std::ostream& os, const PlayState& patternLine);
        };
    }
}