/**
 * @file RollupConfig.hpp
 * @brief Header file for blink1_control::config::RollupConfig
 */

#pragma once

#include <string>
#include <vector>

#include "config/ConditionConfig.hpp"

//TODO operator<<

namespace blink1_control {
    namespace config {

        /**
         * Reference to another ConditionConfig that is used to keep track
         * of children for rollups.
         */
        struct RollupChild {
            /** Name of the child */
            std::string name;

            /** Whether the child is considered critical (see RollupConfig) */
            bool critical;
        };

        /**
         * Configuration for rollups
         *
         * This is an extension of ConditionConfig
         * for the case where the type is ConditionConfig::Type::Rollup
         *
         * Keeps a list of children. If all children are good, runs the
         * first pattern. If only non-critical children are bad, runs
         * the second pattern. Otherwise, runs the third pattern.
         */
        struct RollupConfig : public ConditionConfig {

            /** The child conditions to combine */
            std::vector<RollupChild> children;

            virtual ~RollupConfig() = default;
        };
    }
}
