#include <fstream>
#include <iostream>

#include "config/ConfigParser.hpp"
#include "blink-lib/PatternLineN.hpp"

namespace blink1_control {
    static std::string CONDITIONS_STRING = "conditions";
    static std::string PATTERNS_STRING = "patterns";

    static std::unordered_map<std::string, ConditionConfig::Type> types{
        {"ProcessMonitor", ConditionConfig::Type::ProcessMonitor},
        {"Rollup", ConditionConfig::Type::Rollup}
    };

    std::optional<Config> ConfigParser::parseConfig(std::filesystem::path path) {
        std::ifstream instream(path);
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
            std::cout << "Error opening file: " << path << std::endl;
        }

        return std::nullopt;
    }

    static bool parseArray(const Json& json, Config& config, std::string arrayName, std::function<bool(const Json&, Config&)> parseFunction) {
        bool success = true;

        if (json.find(arrayName) != json.end()) {
            Json conditionsArray = json[arrayName];
            for (auto& conditionJson : conditionsArray) {
                success &= parseFunction(conditionJson, config);
            }
        }

        return success;
    }

    bool ConfigParser::parseConditions(const Json& json, Config& config) const {
        return parseArray(json, config, CONDITIONS_STRING, [this](const Json& ljson, Config& lconfig){return parseCondition(ljson, lconfig);});
    }

    bool ConfigParser::parseCondition(const Json& json, Config& config) const {
        bool success = true;

        try {
            ConditionConfig condition;
            condition.name = json["name"];
            condition.type = types.at(json["type"]);

            for (Json patternName : json["patterns"]) {
                if (patternName.is_string()) {
                    condition.patterns.push_back(patternName);
                } else {
                    std::cout << "Pattern name is not a string: " << patternName << std::endl;
                    success = false;
                }
            }

            if (success) {
                config.conditionConfigs[condition.name] = condition;
            }
        } catch (std::exception& err) {
            std::cout << "Error parsing condition: " << err.what() << std::endl;
            success = false;
        }

        return success;
    }

    bool ConfigParser::parsePatterns(const Json& json, Config& config) const {
        return parseArray(json, config, PATTERNS_STRING, [this](const Json& ljson, Config& lconfig){return parsePattern(ljson, lconfig);});
    }

    bool ConfigParser::parsePattern(const Json& json, Config& config) const {
        bool success = true;

        try {
            PatternConfig pattern;
            pattern.name = json["name"];
            pattern.repeat = json["repeat"];

            for (Json line : json["lines"]) {
                PatternLineN patternLine;
                patternLine.rgbn = parseRgb(line["color"]);
                patternLine.rgbn.n = line["led"];
                patternLine.fadeMillis = line["time"];
                pattern.pattern.push_back(patternLine);
            }

            config.patternConfigs[pattern.name] = pattern;
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

    RGBN ConfigParser::parseRgb(const std::string& rgbString) const {
        RGBN out;
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
