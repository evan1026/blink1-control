/**
 * @file PatternCommand.hpp
 * @brief Header file for blink1_control::config::PatternCommand and derived classes
 */


#pragma once

#include <chrono>

#include "blink-lib.hpp"

namespace blink1_control {
    namespace config {

        /**
         * Abstract class defining a command that can be in a pattern.
         *
         * @see PatternConfig
         */
        struct PatternCommand {
            virtual ~PatternCommand() {}

            /**
             * Executes the command.
             *
             * @param device The blink(1) device to execute on, if applicable.
             */
            virtual void execute(blink1_lib::Blink1Device& device) = 0;

            /**
             * Output operator
             *
             * @param os Output stream
             * @param config PatternCommand to output
             */
            friend std::ostream& operator<<(std::ostream& os, const blink1_control::config::PatternCommand& config) {
                config.output(os);
                return os;
            }

            private:
                /**
                 * Actual implementation of output operator
                 *
                 * @param os Output stream
                 */
                virtual void output(std::ostream& os) const = 0;
        };

        /**
         * Fades the blink(1) device (1 or all LEDs, depending on the value of N in fadeParams)
         * to a new color according to the parameters passed in the constructor
         */
        struct FadeCommand : public PatternCommand {

            /**
             * All information needed to do the fade is held in this object
             */
            blink1_lib::PatternLineN fadeParams;

            /**
             * Constructor
             *
             * @param fadeParams Information about the fade (color and duration)
             */
            FadeCommand(blink1_lib::PatternLineN fadeParams);

            /**
             * Executes the fade command on the given device.
             *
             * @param device The blink(1) device to execute on
             */
            void execute(blink1_lib::Blink1Device& device);

            private:
                /**
                 * Actual implementation of output operator
                 *
                 * @param os Output stream
                 */
                void output(std::ostream& os) const {
                    os << "FadeCommand{fadeParams: " << fadeParams << "}";
                }
        };

        /**
         * Simply sleeps for the specified time. Used to insert delays into patterns.
         */
        struct WaitCommand : public PatternCommand {

            /**
             * How long to wait
             */
            std::chrono::milliseconds waitTime;

            /**
             * Constructor.
             *
             * @param waitTime How long to wait
             */
            WaitCommand(std::chrono::milliseconds waitTime);

            /**
             * Executes the sleep.
             *
             * @param device Ignored
             */
            void execute(blink1_lib::Blink1Device& device);

            private:
                /**
                 * Actual implementation of output operator
                 *
                 * @param os Output stream
                 */
                void output(std::ostream& os) const {
                    os << "WaitCommand{waitTime: " << waitTime.count() << "ms}";
                }
        };

    }
}
