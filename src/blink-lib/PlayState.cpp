#include "blink-lib/PlayState.hpp"

namespace blink1_control::blink1_lib {
    PlayState::PlayState(const bool _playing, const std::uint8_t _playStart, const std::uint8_t _playEnd, const std::uint8_t _playCount, const std::uint8_t _playPos) noexcept
        : playing(_playing), playStart(_playStart), playEnd(_playEnd), playCount(_playCount), playPos(_playPos)
        {}

    bool PlayState::operator==(const PlayState& other) const noexcept {
        return playing == other.playing
            && playStart == other.playStart
            && playEnd == other.playEnd
            && playCount == other.playCount
            && playPos == other.playPos;
    }

    bool PlayState::operator!=(const PlayState& other) const noexcept {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const PlayState& playState) {
        os << "PlayState{"
            << "playing="     << (playState.playing ? "true" : "false")
            << ", playStart=" << unsigned(playState.playStart)
            << ", playEnd="   << unsigned(playState.playEnd)
            << ", playCount=" << unsigned(playState.playCount)
            << ", playPos="   << unsigned(playState.playPos)
            << "}";
        return os;
    }
}
