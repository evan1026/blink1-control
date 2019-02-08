#include "PatternLine.hpp"
namespace blink1_control {
    PatternLine::PatternLine() : fadeMillis(), rgb() {}
    PatternLine::PatternLine(RGB _rgb, std::uint16_t _fadeMillis) : fadeMillis(_fadeMillis), rgb(_rgb) {}
    PatternLine::PatternLine(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint16_t _fadeMillis) : fadeMillis(_fadeMillis), rgb(_r, _g, _b) {}

    bool PatternLine::operator==(const PatternLine& other) const {
        return fadeMillis == other.fadeMillis && rgb == other.rgb;
    }

    bool PatternLine::operator!=(const PatternLine& other) const {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine) {
        os << "PatternLine{rgb=" << patternLine.rgb << ", fadeMillis=" << unsigned(patternLine.fadeMillis) << "}";
        return os;
    }
}
