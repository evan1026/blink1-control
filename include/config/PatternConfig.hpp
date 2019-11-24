#include <vector>
#include <string>

#include "blink-lib/PatternLine.hpp"

namespace blink1_control {
    struct PatternConfig {
        std::string ledConfig;
        std::vector<PatternLine> pattern;
    };
}
