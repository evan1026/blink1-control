#include <string>

namespace blink1_control {
    struct ConditionConfig {
        enum class Type {
            ProcessMonitor,
            Rollup
        };

        Type type;
        std::string name;
    };
}
