/**
 * @file Config.hpp
 * @brief Header file for blink1_control::config::Config
 */

#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <string>

#include "config/ConditionConfig.hpp"
#include "config/PatternConfig.hpp"

namespace blink1_control {
    namespace config {

        /**
         * The configuration for this application
         */
        struct Config {

            /**
             * A collection of pattern configurations which maps from the name
             * of a PatternConfig to the PatternConfig itself
             */
            std::map<std::string, std::shared_ptr<PatternConfig>> patternConfigs;

            /**
             * A collection of condition configurations which maps from the name
             * of a ConditionConfig to the ConditionConfig itself
             */
            std::map<std::string, std::shared_ptr<ConditionConfig>> conditionConfigs;

            /**
             * Output operator
             *
             * @param os Output stream
             * @param config Config to output
             */
            friend std::ostream& operator<<(std::ostream& os, blink1_control::config::Config& config) {
                os << "{patterns: {";
                bool first = true;
                for (auto entryPair : config.patternConfigs) {
                    if (first) {
                        first = false;
                    } else {
                        os << ", ";
                    }
                    os << entryPair.first << ": " << *entryPair.second;
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
                    os << entryPair.first << ": " << *entryPair.second;
                }
                os << "}}";

                return os;
            }
        };
    }
}
