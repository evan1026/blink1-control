#include <string>
#include <vector>

#include "config/PatternConfig.hpp"

namespace blink1_control {
    struct ConditionConfig {
        enum class Type {
            ProcessMonitor,
            Rollup
        };

        Type type;
        std::string name;
        std::vector<std::string> patterns;

        friend std::ostream& operator<<(std::ostream& os, blink1_control::ConditionConfig::Type& configType) {
            switch (configType) {
                case blink1_control::ConditionConfig::Type::ProcessMonitor:
                    os << "ProcessMonitor";
                    break;
                case blink1_control::ConditionConfig::Type::Rollup:
                    os << "Rollup";
                    break;
            }

            return os;
        }

        friend std::ostream& operator<<(std::ostream& os, blink1_control::ConditionConfig& config) {
            os << "{type: " << config.type << ", name: " << config.name << ", patterns: {";
            bool first = true;
            for (auto pattern : config.patterns) {
                if (first) {
                    first = false;
                } else {
                    os << ", ";
                }
                os << pattern;
            }
            os << "}}";

            return os;
        }
    };
}

