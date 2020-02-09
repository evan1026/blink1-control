#include "blink-lib/PatternLineN.hpp"

namespace blink1_control {
    namespace blink1_lib {
        PatternLineN::PatternLineN() noexcept : fadeMillis(0), rgbn() {}
        PatternLineN::PatternLineN(const RGBN& _rgbn, const std::uint16_t _fadeMillis) noexcept : fadeMillis(_fadeMillis), rgbn(_rgbn) {}
        PatternLineN::PatternLineN(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint8_t _n, const std::uint16_t _fadeMillis) noexcept
            : fadeMillis(_fadeMillis), rgbn(_r, _g, _b, _n) {}

        bool PatternLineN::operator==(const PatternLineN& other) const noexcept {
            return fadeMillis == other.fadeMillis && rgbn == other.rgbn;
        }

        bool PatternLineN::operator!=(const PatternLineN& other) const noexcept {
            return !(*this == other);
        }

        std::ostream& operator<<(std::ostream& os, const PatternLineN& patternLine) {
            os << "PatternLine{rgbn=" << patternLine.rgbn << ", fadeMillis=" << unsigned(patternLine.fadeMillis) << "}";
            return os;
        }
    }
}
