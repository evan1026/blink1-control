/**
 * @file ProcessMonitorConfig.hpp
 * @brief Header file for blink1_control::config::ProcessMonitorConfig
 */

#pragma once

#include <string>

#include "config/ConditionConfig.hpp"

//TODO operator<<

namespace blink1_control::config {

    /**
     * Configuration for process monitors
     *
     * This is an extension of ConditionConfig
     * for the case where the type is ConditionConfig::Type::ProcessMonitor
     *
     * The state of the process monitor is based on running a command (stored
     * in ProcessMonitorConfig::cmd).
     *
     * If the process returns 0 then the monitor considers it good. If it
     * is non-zero then it is considered bad. If there are patterns on
     * this condition, there must be exactly two, and the first will
     * be used for the good case while the second will used for the bad case.
     */
    struct ProcessMonitorConfig : public ConditionConfig {

        /**
         * The command to run to monitor.
         */
        std::string cmd;

        virtual ~ProcessMonitorConfig() override = default;
    };
}
