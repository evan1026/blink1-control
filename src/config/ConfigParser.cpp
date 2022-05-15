#include <iostream>
#include <sstream>

#include "blink-lib.hpp"

#include "config/ConfigParser.hpp"
#include "config/ProcessMonitorConfig.hpp"
#include "config/RollupConfig.hpp"

namespace blink1_control::config {
    static const std::string CONDITIONS_STRING = "conditions";
    static const std::string PATTERNS_STRING = "patterns";

    static const std::unordered_map<std::string, ConditionConfig::Type> types{
        {"ProcessMonitor", ConditionConfig::Type::ProcessMonitor},
        {"Rollup", ConditionConfig::Type::Rollup}
    };

    std::optional<Config> ConfigParser::parseConfig(std::istream& instream) {
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

    static bool parseArray(const Json& json, Config& config, const std::string& arrayName, const std::function<bool(const Json&, Config&)>& parseFunction) {
        bool success = true;

        if (json.find(arrayName) != json.end()) {
            Json conditionsArray = json.at(arrayName);
            for (auto& conditionJson : conditionsArray) {
                success &= parseFunction(conditionJson, config);
            }
        }

        return success;
    }

    bool ConfigParser::parseConditions(const Json& json, Config& config) {
        return parseArray(json, config, CONDITIONS_STRING, [](const Json& ljson, Config& lconfig){return parseCondition(ljson, lconfig);});
    }

    bool ConfigParser::parseCondition(const Json& json, Config& config) {
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

            for (const Json& patternName : json.at("patterns")) {
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

    std::shared_ptr<ProcessMonitorConfig> ConfigParser::parseProcessMonitor(const Json& json) {
        auto pm_config = std::make_shared<ProcessMonitorConfig>();
        pm_config->cmd = json.at("cmd");
        return pm_config;
    }

    std::shared_ptr<RollupConfig> ConfigParser::parseRollup(const Json& json) {
        auto rollupConfig = std::make_shared<RollupConfig>();
        for (Json childConfig : json.at("children")) {
            RollupChild childCondition;
            childCondition.name = childConfig.at("name");
            childCondition.critical = childConfig.at("critical");
            rollupConfig->children.push_back(childCondition);
        }
        return rollupConfig;
    }

    bool ConfigParser::parsePatterns(const Json& json, Config& config) {
        return parseArray(json, config, PATTERNS_STRING, [](const Json& ljson, Config& lconfig){return parsePattern(ljson, lconfig);});
    }

    bool ConfigParser::parsePattern(const Json& json, Config& config) {
        bool success = true;

        try {
            std::shared_ptr<PatternConfig> pattern = std::make_shared<PatternConfig>();
            pattern->name = json.at("name");
            pattern->repeat = json.at("repeat");

            for (const Json& line : json.at("lines")) {
                bool hasLed = line.contains("led");
                bool hasColor = line.contains("color");

                if (hasLed && hasColor) {
                    blink1_lib::PatternLineN patternLine;
                    patternLine.rgbn = parseRgb(line.at("color"));
                    patternLine.rgbn.n = line.at("led");
                    patternLine.fadeMillis = line.at("time");

                    pattern->pattern.push_back(std::make_unique<FadeCommand>(patternLine));
                } else if (hasLed || hasColor) {
                    throw std::runtime_error("Pattern line must contain both 'led' and 'color' or neither of them");
                } else {
                    pattern->pattern.push_back(std::make_unique<WaitCommand>(std::chrono::milliseconds(line.at("time"))));
                }
            }

            config.patternConfigs.emplace(pattern->name, pattern);
        } catch (std::exception& err) {
            std::cout << "Error parsing pattern: " << err.what() << std::endl;
            success = false;
        }

        return success;
    }

    static std::uint8_t parseHexString(const std::string& string) {
        unsigned int x = 0;
        std::stringstream ss;
        ss << std::hex << string;
        ss >> x;

        return static_cast<std::uint8_t>(x);
    }

    blink1_lib::RGBN ConfigParser::parseRgb(const std::string& rgbString) {
        constexpr int RGB_STR_LEN = 7;
        constexpr int COLOR_LEN = 2;
        constexpr int RED_START = 1;
        constexpr int GREEN_START = 3;
        constexpr int BLUE_START = 5;

        blink1_lib::RGBN out;
        if (rgbString.rfind('#', 0) == 0 && rgbString.length() == RGB_STR_LEN) {
            std::string redString = rgbString.substr(RED_START, COLOR_LEN);
            std::string greenString = rgbString.substr(GREEN_START, COLOR_LEN);
            std::string blueString = rgbString.substr(BLUE_START, COLOR_LEN);

            out.r = parseHexString(redString);
            out.g = parseHexString(greenString);
            out.b = parseHexString(blueString);
        } else {
            throw std::runtime_error("Cannot parse RGB string: " + rgbString);
        }

        return out;
    }
}
