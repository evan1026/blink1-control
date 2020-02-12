/**
 * @file ConditionConfig.hpp
 * @brief Header file for blink1_control::config::ConditionConfig
 */

#pragma once

#include <string>
#include <vector>

#include "config/PatternConfig.hpp"

namespace blink1_control {
    namespace config {

        /**
         * Configuration for conditions
         *
         * A condition is basically just a situation that can trigger
         * an LED pattern to play.
         *
         * Currently 2 types: ConditionConfig::Type::ProcessMonitor
         * and ConditionConfig::Type::Rollup. Each is individually documented
         */
        struct ConditionConfig {

            /**
             * Represents the Type of a condition. The Type basically decides
             * how to turn states in the system into patterns for the LED
             */
            enum class Type {
                /**
                 * This condition will run a process periodically and
                 * check the return value of the process. Will trigger
                 * different patterns for a "pass" state vs a "fail"
                 * state. The pass and fail states also go to the
                 * Rollup type
                 */
                ProcessMonitor,

                /**
                 * A Rollup of other types basically takes the component
                 * conditions and combines them. If they are all passed,
                 * then the Rollup is passed. If one is failed, then the
                 * Rollup is failed.
                 */
                Rollup
            };

            /**
             * The Type of this condition
             */
            Type type;

            /**
             * The name of this condition
             */
            std::string name;

            /**
             * The list of patterns that can get activated based on the state of the condition
             */
            std::vector<std::string> patterns;

            /**
             * Output operator
             *
             * @param os Output stream
             * @param configType ConditionConfig::Type to output
             */
            friend std::ostream& operator<<(std::ostream& os, blink1_control::config::ConditionConfig::Type& configType) {
                switch (configType) {
                    case blink1_control::config::ConditionConfig::Type::ProcessMonitor:
                        os << "ProcessMonitor";
                        break;
                    case blink1_control::config::ConditionConfig::Type::Rollup:
                        os << "Rollup";
                        break;
                }

                return os;
            }

            /**
             * Output operator
             *
             * @param os Output stream
             * @param config Config to output
             */
            friend std::ostream& operator<<(std::ostream& os, blink1_control::config::ConditionConfig& config) {
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
}
