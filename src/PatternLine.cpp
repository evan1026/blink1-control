#include "PatternLine.hpp"
namespace blink1_control {
    PatternLine::PatternLine() noexcept : fadeMillis(), rgb() {}
    PatternLine::PatternLine(const RGB& _rgb, const std::uint16_t _fadeMillis) noexcept : fadeMillis(_fadeMillis), rgb(_rgb) {}
    PatternLine::PatternLine(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint16_t _fadeMillis) noexcept
        : fadeMillis(_fadeMillis), rgb(_r, _g, _b) {}

    bool PatternLine::operator==(const PatternLine& other) const noexcept {
        return fadeMillis == other.fadeMillis && rgb == other.rgb;
    }

    bool PatternLine::operator!=(const PatternLine& other) const noexcept {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const PatternLine& patternLine) {
        os << "PatternLine{rgb=" << patternLine.rgb << ", fadeMillis=" << unsigned(patternLine.fadeMillis) << "}";
        return os;
    }
}
