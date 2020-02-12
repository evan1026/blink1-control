/**
 * @file ConfigParser.hpp
 * @brief Header file for blink1_control::config::ConfigParser
 */

#pragma once

#include <optional>

#include <nlohmann/json.hpp>

#include "config/Config.hpp"
#include "blink-lib/PatternLineN.hpp"

// @cond DO_NOT_DOCUMENT
using Json = nlohmann::json;
// @endcond

namespace blink1_control {
    namespace config {

        /**
         * Parses a json config file and turns it into a Config object
         *
         * Contains a single public function - parseConfig(std::istream&)
         */
        class ConfigParser {
            [[nodiscard]] bool parseConditions(const Json& json, Config& config) const noexcept;
            [[nodiscard]] bool parseCondition(const Json& json, Config& config) const noexcept;
            [[nodiscard]] bool parsePatterns(const Json& json, Config& config) const noexcept;
            [[nodiscard]] bool parsePattern(const Json& json, Config& config) const;
            [[nodiscard]] blink1_lib::RGBN parseRgb(const std::string& rgbString) const;

            public:
                /**
                 * Parses data in the istream as json data and creates a Config object from it
                 *
                 * @param instream Input stream containing the configuration data
                 *                 as a json string
                 * @return The parsed Config if it could be parsed, std::nullopt otherwise
                 */
                std::optional<Config> parseConfig(std::istream& instream) noexcept;
        };
    }
}
