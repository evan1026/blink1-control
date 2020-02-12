/**
 * @file PatternConfig.hpp
 * @brief Header file for blink1_control::config::PatternConfig
 */

#pragma once

#include <vector>
#include <string>

#include "blink-lib/PatternLineN.hpp"

namespace blink1_control {
    namespace config {

        /**
         * Configuration values for a pattern which can play on the device
         */
        struct PatternConfig {

            /**
             * The name of this pattern config
             */
            std::string name;

            /**
             * A list of pattern lines which make up the pattern
             */
            std::vector<blink1_lib::PatternLineN> pattern;

            /**
             * Number of times to repeat the pattern
             */
            int repeat;

            /**
             * Output operator
             *
             * @param os Output stream
             * @param config PatternConfig to output
             */
            friend std::ostream& operator<<(std::ostream& os, blink1_control::config::PatternConfig& config) {
                os << "{name: " << config.name << ", repeat: " << config.repeat << ", pattern: {";
                bool first = true;
                for (auto configPattern : config.pattern) {
                    if (first) {
                        first = false;
                    } else {
                        os << ", ";
                    }
                    os << configPattern;
                }
                os << "}}";

                return os;
            }
        };
    }
}
