#include <vector>

#include "config/PatternConfig.hpp"
#include "config/LedConfig.hpp"
#include "config/ConditionConfig.hpp"

namespace blink1_control {
    struct Config {
        std::vector<PatternConfig> patternConfigs;
        std::vector<LedConfig> ledConfigs;
        std::vector<ConditionConfig> conditionConfigs;
    };
}
