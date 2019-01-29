#pragma once

#include <cstdint>

namespace blink1_control {
    struct PlayState {
        bool playing;
        std::uint8_t playStart;
        std::uint8_t playEnd;
        std::uint8_t playCount;
        std::uint8_t playPos;
    };
}
