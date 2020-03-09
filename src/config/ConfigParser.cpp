#include <iostream>
#include <sstream>

#include "blink-lib/PatternLineN.hpp"
#include "config/ConfigParser.hpp"
#include "config/ProcessMonitorConfig.hpp"
#include "config/RollupConfig.hpp"

namespace blink1_control {
    namespace config {
        static std::string CONDITIONS_STRING = "conditions";
        static std::string PATTERNS_STRING = "patterns";

        static std::unordered_map<std::string, ConditionConfig::Type> types{
            {"ProcessMonitor", ConditionConfig::Type::ProcessMonitor},
            {"Rollup", ConditionConfig::Type::Rollup}
        };

        std::optional<Config> ConfigParser::parseConfig(std::istream& instream) noexcept {
            Json json;

            if (instream.good()) {
                try {
                    instream >> json;

                    Config config;
                    bool success = true;
                    success &= parseConditions(json, config);
                    success &= parsePatterns(json, config);

                    if (success) {
                        return std::make_optional(config);
                    }
                } catch (nlohmann::detail::exception& err) {
                    std::cout << "Error parsing json: " << err.what() << std::endl;
                }
            } else {
                std::cout << "Error reading from stream" << std::endl;
            }

            return std::nullopt;
        }

        static bool parseArray(const Json& json, Config& config, std::string arrayName, std::function<bool(const Json&, Config&)> parseFunction) {
            bool success = true;

            if (json.find(arrayName) != json.end()) {
                Json conditionsArray = json.at(arrayName);
                for (auto& conditionJson : conditionsArray) {
                    success &= parseFunction(conditionJson, config);
                }
            }

            return success;
        }

        bool ConfigParser::parseConditions(const Json& json, Config& config) const noexcept {
            return parseArray(json, config, CONDITIONS_STRING, [this](const Json& ljson, Config& lconfig){return parseCondition(ljson, lconfig);});
        }

        bool ConfigParser::parseCondition(const Json& json, Config& config) const noexcept {
            bool success = true;

            try {
                std::shared_ptr<ConditionConfig> condition;

                auto type = types.at(json.at("type"));
                switch (type) {
                    case ConditionConfig::Type::ProcessMonitor:
                        condition = parseProcessMonitor(json);
                        break;
                    case ConditionConfig::Type::Rollup:
                        condition = parseRollup(json);
                        break;
                }

                condition->name = json.at("name");
                condition->type = type;

                for (Json patternName : json.at("patterns")) {
                    if (patternName.is_string()) {
                        condition->patterns.push_back(patternName);
                    } else {
                        std::cout << "Pattern name is not a string: " << patternName << std::endl;
                        success = false;
                    }
                }

                if (success) {
                    config.conditionConfigs.insert_or_assign(condition->name, condition);
                }
            } catch (std::exception& err) {
                std::cout << "Error parsing condition: " << err.what() << std::endl;
                success = false;
            }

            return success;
        }

        std::shared_ptr<ProcessMonitorConfig> ConfigParser::parseProcessMonitor(const Json& json) const {
            auto pm_config = std::make_shared<ProcessMonitorConfig>();
            pm_config->cmd = json.at("cmd");
            return pm_config;
        }

        std::shared_ptr<RollupConfig> ConfigParser::parseRollup(const Json& json) const {
            auto rollupConfig = std::make_shared<RollupConfig>();
            for (Json childConfig : json.at("children")) {
                RollupChild childCondition;
                childCondition.name = childConfig.at("name");
                childCondition.critical = childConfig.at("critical");
                rollupConfig->children.push_back(childCondition);
            }
            return rollupConfig;
        }

        bool ConfigParser::parsePatterns(const Json& json, Config& config) const noexcept {
            return parseArray(json, config, PATTERNS_STRING, [this](const Json& ljson, Config& lconfig){return parsePattern(ljson, lconfig);});
        }

        bool ConfigParser::parsePattern(const Json& json, Config& config) const {
            bool success = true;

            try {
                std::shared_ptr<PatternConfig> pattern = std::make_shared<PatternConfig>();
                pattern->name = json.at("name");
                pattern->repeat = json.at("repeat");

                for (Json line : json.at("lines")) {
                    blink1_lib::PatternLineN patternLine;
                    patternLine.rgbn = parseRgb(line.at("color"));
                    patternLine.rgbn.n = line.at("led");
                    patternLine.fadeMillis = line.at("time");
                    pattern->pattern.push_back(patternLine);
                }

                config.patternConfigs.emplace(pattern->name, pattern);
            } catch (std::exception& err) {
                std::cout << "Error parsing pattern: " << err.what() << std::endl;
                success = false;
            }

            return success;
        }

        static std::uint8_t parseHexString(const std::string& string) {
            unsigned int x;
            std::stringstream ss;
            ss << std::hex << string;
            ss >> x;

            return static_cast<std::uint8_t>(x);
        }

        blink1_lib::RGBN ConfigParser::parseRgb(const std::string& rgbString) const {
            blink1_lib::RGBN out;
            if (rgbString.rfind("#", 0) == 0 && rgbString.length() == 7) {
                std::string redString = rgbString.substr(1, 2);
                std::string greenString = rgbString.substr(3, 2);
                std::string blueString = rgbString.substr(5, 2);

                out.r = parseHexString(redString);
                out.g = parseHexString(greenString);
                out.b = parseHexString(blueString);
            } else {
                throw std::runtime_error("Cannot parse RGB string: " + rgbString);
            }

            return out;
        }
    }
}
