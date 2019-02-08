#include "PatternLineN.hpp"

namespace blink1_control {
    PatternLineN::PatternLineN() : fadeMillis(0), rgbn() {}
    PatternLineN::PatternLineN(RGBN _rgbn, std::uint16_t _fadeMillis) : fadeMillis(_fadeMillis), rgbn(_rgbn) {}
    PatternLineN::PatternLineN(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _n, std::uint16_t _fadeMillis) : fadeMillis(_fadeMillis), rgbn(_r, _g, _b, _n) {}

    bool PatternLineN::operator==(const PatternLineN& other) const {
        return fadeMillis == other.fadeMillis && rgbn == other.rgbn;
    }

    bool PatternLineN::operator!=(const PatternLineN& other) const {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine) {
        os << "PatternLine{rgbn=" << patternLine.rgbn << ", fadeMillis=" << unsigned(patternLine.fadeMillis) << "}";
        return os;
    }
}
