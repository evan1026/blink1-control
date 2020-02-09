#pragma once

#include <optional>

#include <nlohmann/json.hpp>

#include "config/Config.hpp"
#include "blink-lib/PatternLineN.hpp"

using Json = nlohmann::json;

namespace blink1_control {
    namespace config {
        class ConfigParser {
            [[nodiscard]] bool parseConditions(const Json& json, Config& config) const noexcept;
            [[nodiscard]] bool parseCondition(const Json& json, Config& config) const noexcept;
            [[nodiscard]] bool parsePatterns(const Json& json, Config& config) const noexcept;
            [[nodiscard]] bool parsePattern(const Json& json, Config& config) const;
            [[nodiscard]] blink1_lib::RGBN parseRgb(const std::string& rgbString) const;

            public:
                std::optional<Config> parseConfig(std::istream& instream) noexcept;
        };
    }
}
