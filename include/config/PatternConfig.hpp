#pragma once

#include <vector>
#include <string>

#include "blink-lib/PatternLineN.hpp"

namespace blink1_control {
    namespace config {
        struct PatternConfig {
            std::string name;
            std::vector<blink1_lib::PatternLineN> pattern;
            int repeat;

            friend std::ostream& operator<<(std::ostream& os, blink1_control::config::PatternConfig& config) {
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
}
