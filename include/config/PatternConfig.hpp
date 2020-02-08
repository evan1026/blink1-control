#pragma once

#include <vector>
#include <string>

#include "blink-lib/PatternLineN.hpp"

namespace blink1_control {
    struct PatternConfig {
        std::string name;
        std::vector<PatternLineN> pattern;
        int repeat;

        friend std::ostream& operator<<(std::ostream& os, blink1_control::PatternConfig& config) {
            os << "{name: " << config.name << ", repeat: " << config.repeat << ", pattern: {";
            bool first = true;
            for (auto configPattern : config.pattern) {
                if (first) {
                    first = false;
                } else {
                    os << ", ";
                }
                os << configPattern;
            }
            os << "}}";

            return os;
        }
    };
}
