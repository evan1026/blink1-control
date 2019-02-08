#pragma once

#include <cstdint>
#include <ostream>

namespace blink1_control {
    struct PlayState {
        bool playing;
        std::uint8_t playStart;
        std::uint8_t playEnd;
        std::uint8_t playCount;
        std::uint8_t playPos;

        PlayState();
        PlayState(bool playing, std::uint8_t playStart, std::uint8_t playEnd, std::uint8_t playCount, std::uint8_t playPos);

        bool operator==(const PlayState& other) const;
        bool operator!=(const PlayState& other) const;

        friend std::ostream& operator<<(std::ostream& os, const PlayState& patternLine);
    };
}
