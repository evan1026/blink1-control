/**
 * @file ConfigParser.hpp
 * @brief Header file for blink1_control::config::ConfigParser
 */

#pragma once

#include <optional>

#include <nlohmann/json.hpp>

#include "blink-lib.hpp"

#include "config/Config.hpp"
#include "config/ProcessMonitorConfig.hpp"
#include "config/RollupConfig.hpp"

// @cond DO_NOT_DOCUMENT
using Json = nlohmann::json;
// @endcond

namespace blink1_control::config {

    /**
     * Parses a json config file and turns it into a Config object
     *
     * Contains a single public function - parseConfig(std::istream&)
     */
    class ConfigParser {
        [[nodiscard]] static bool parseConditions(const Json& json, Config& config);
        [[nodiscard]] static bool parseCondition(const Json& json, Config& config);
        [[nodiscard]] static bool parsePatterns(const Json& json, Config& config);
        [[nodiscard]] static bool parsePattern(const Json& json, Config& config);
        [[nodiscard]] static bool parseTopLevelVars(const Json& json, Config& config);
        [[nodiscard]] static std::shared_ptr<ProcessMonitorConfig> parseProcessMonitor(const Json& json);
        [[nodiscard]] static std::shared_ptr<RollupConfig> parseRollup(const Json& json);
        [[nodiscard]] static blink1_lib::RGBN parseRgb(const std::string& rgbString);
        static void readPattern(const Json& json, std::vector<std::unique_ptr<PatternCommand>>& commands);

        public:
            /**
             * Parses data in the istream as json data and creates a Config object from it
             *
             * @param instream Input stream containing the configuration data
             *                 as a json string
             * @return The parsed Config if it could be parsed, std::nullopt otherwise
             */
            [[nodiscard]] static std::optional<Config> parseConfig(std::istream& instream);
    };
}
