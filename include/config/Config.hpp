#pragma once

#include <map>
#include <string>

#include "config/PatternConfig.hpp"
#include "config/ConditionConfig.hpp"

namespace blink1_control {
    struct Config {
        std::map<std::string, PatternConfig> patternConfigs;
        std::map<std::string, ConditionConfig> conditionConfigs;

        friend std::ostream& operator<<(std::ostream& os, blink1_control::Config& config) {
            os << "{patterns: {";
            bool first = true;
            for (auto entryPair : config.patternConfigs) {
                if (first) {
                    first = false;
                } else {
                    os << ", ";
                }
                os << entryPair.first << ": " << entryPair.second;
            }
            os << "}, ";

            os << "conditions: {";
            first = true;
            for (auto entryPair : config.conditionConfigs) {
                if (first) {
                    first = false;
                } else {
                    os << ", ";
                }
                os << entryPair.first << ": " << entryPair.second;
            }
            os << "}}";

            return os;
        }
    };
}

