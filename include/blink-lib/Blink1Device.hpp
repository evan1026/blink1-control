/**
 * @file Blink1Device.hpp
 * @brief Header file for blink1_control::blink1_lib::Blink1Device
 */

#pragma once

#include <blink1-lib.h>
#include <functional>
#include <memory>
#include <optional>

#include "blink-lib/PatternLine.hpp"
#include "blink-lib/PatternLineN.hpp"
#include "blink-lib/PlayState.hpp"
#include "blink-lib/RGB.hpp"
#include "blink-lib/RGBN.hpp"

namespace blink1_control {
    namespace blink1_lib {

        /**
         * A wrapper around the blink1 C library used to control blink1 devices
         */
        class Blink1Device {
            std::unique_ptr<blink1_device, std::function<void(blink1_device*)>> device;

            static void destroyBlinkDevice(blink1_device* device) noexcept;

            public:
                /**
                 * Defines how to interpret the string initializer passed into the constructors
                 * that take strings
                 */
                enum class STRING_INIT_TYPE {
                    /** Interpret the initializer as a path to the device */
                    PATH,
                    /** Interpret the initializer as a serial ID */
                    SERIAL
                };

                /**
                 * Default constructor
                 */
                Blink1Device() noexcept;

                /**
                 * @param id The ID of the device to initialize
                 */
                Blink1Device(const std::uint32_t id) noexcept;

                /**
                 * @param stringInitializer String to select the device to initialize
                 * @param initType How to interpret the the string
                 */
                Blink1Device(const std::string& stringInitializer, const STRING_INIT_TYPE initType) noexcept;

                /**
                 * @param stringInitializer String to select the device to initialize
                 * @param initType How to interpret the the string
                 */
                Blink1Device(const char* stringInitializer, const STRING_INIT_TYPE initType) noexcept;

                /**
                 * Returns whether or not the device is successfully connected
                 *
                 * @return true if the device is connected and ready, false otherwise
                 */
                [[nodiscard]] bool good() const noexcept;

                /**
                 * Returns the same thing as good()
                 *
                 * @return Whatever good() returns
                 * @see good()
                 */
                [[nodiscard]] operator bool() const noexcept;

                /**
                 * Gets the version of the device that is connected. Returns std::nullopt if good() returns false
                 *
                 * @return The version of the device
                 */
                [[nodiscard]] std::optional<int> getVersion() const noexcept;

                /**
                 * Fades the device to another color over time. If this device has multiple LEDs,
                 * all LEDs will fade to that color.
                 *
                 * @param fadeMillis The amount of time in milliseconds for the fade to last
                 * @param rgb RGB color to fade to
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool fadeToRGB(const std::uint16_t fadeMillis, const RGB& rgb) noexcept;

                /**
                 * Fades the device to another color over time. Only fades the LED specified in
                 * the RGBN value.
                 *
                 * @param fadeMillis The amount of time in milliseconds for the fade to last
                 * @param rgbn RGB color to fade to along with which LED on the device to fade to
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool fadeToRGBN(const std::uint16_t fadeMillis, const RGBN& rgbn) noexcept;

                /**
                 * Sets the color of all LEDs on the device. The same as calling
                 * fadeToRGB(const std::uint16_t, const RGB&) with fadeMillis set to 0.
                 *
                 * @param rgb The color to set
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool setRGB(const RGB& rgb) noexcept;

                /**
                 * Sets the color of the specified on the device. The same as calling
                 * fadeToRGBN(const std::uint16_t, const RGBN&) with fadeMillis set to 0.
                 *
                 * @param rgbn The color to set along with which LED to set it on
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool setRGBN(const RGBN& rgbn) noexcept;

                /**
                 * Reads the RGB value of the given LED & the fade time
                 *
                 * @note The underlying function takes in fadeMillis as a parameter,
                 *       but it's not documented at all. This makes me think the function
                 *       might return the target RGB value rather than the current value,
                 *       but the docs say current, so I'm really not sure
                 *
                 * @param ledn The index of the LED to read
                 *
                 * @return A PatternLine (an RGB plus a fade time) if the value could be read successfully, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<PatternLine> readRGBWithFade(const std::uint8_t ledn) const noexcept;

                /**
                 * Reads the RGB value of the given LED
                 *
                 * @note The underlying function takes in fadeMillis as a parameter,
                 *       but it's not documented at all. This makes me think the function
                 *       might return the target RGB value rather than the current value,
                 *       but the docs say current, so I'm really not sure
                 *
                 * @param ledn The index of the LED to read
                 *
                 * @return The RGB value if it was read successfully, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<RGB> readRGB(const std::uint8_t ledn) const noexcept;

                /**
                 * Plays a programmed pattern on the device
                 *
                 * @param pos Position to start playing from
                 * @see writePatternLine(const PatternLine&, const std::uint8_t)
                 * @see writePatternLineN(const PatternLineN&, const std::uint8_t)
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool play(const std::uint8_t pos) noexcept;

                /**
                 * Plays a stored pattern on a loop
                 *
                 * @param startpos Start position for the loop
                 * @param endpos End position for the loop
                 * @param count Number of times to repeat (0 to repeat forever)
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool playLoop(const std::uint8_t startpos, const std::uint8_t endpos, const std::uint8_t count) noexcept;

                /**
                 * Stop playing the pattern that was playing
                 *
                 * @return true if the command was successfully sent to the device, false otherwise
                 */
                bool stop() noexcept;

                /**
                 * Reads the current PlayState of the device
                 *
                 * @return The PlayState if it could be read, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<PlayState> readPlayState() const noexcept;

                /**
                 * Writes a PatternLine to the device at the specified position
                 *
                 * @note On mk2 devices, this saves the pattern to volatile memory. Call savePattern()
                 *       to save the pattern in volatile memory into non-volatile memory
                 *
                 * @param line The line to write
                 * @param pos The position to write the line to - a number from 0 to max_patt (no documentation on what the value is)
                 * @see savePattern()
                 *
                 * @return true if the command was sent successfully to the device, false otherwise
                 */
                bool writePatternLine(const PatternLine& line, const std::uint8_t pos) noexcept;

                /**
                 * Writes a PatternLineN to the device at the specified position
                 *
                 * @note On mk2 devices, this saves the pattern to volatile memory. Call savePattern()
                 *       to save the pattern in volatile memory into non-volatile memory
                 *
                 * @param line The line to write
                 * @param pos The position to write the line to - a number from 0 to max_patt (no documentation on what the value is)
                 * @see savePattern()
                 *
                 * @return true if the command was sent successfully to the device, false otherwise
                 */
                bool writePatternLineN(const PatternLineN& line, const std::uint8_t pos) noexcept;

                /**
                 * Reads the pattern line stored at the given position
                 *
                 * @note The documentation doesn't say whether this reads from volatile or
                 *       non-volatile memory, but I'd assume is the volatile memory
                 *
                 * @param pos The position to read
                 *
                 * @return The PatternLine if it could be read successfully, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<PatternLine> readPatternLine(const std::uint8_t pos) const noexcept;

                /**
                 * Reads the pattern line stored at the given position
                 *
                 * @note The documentation doesn't say whether this reads from volatile or
                 *       non-volatile memory, but I'd assume is the volatile memory
                 *
                 * @param pos The position to read
                 *
                 * @return The PatternLineN if it could be read successfully, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<PatternLineN> readPatternLineN(const std::uint8_t pos) const noexcept;

                /**
                 * Saves the pattern in volatile memory into the non-volatile storage
                 *
                 * @note Based on the documentation, this probably will always return a failure due
                 *       to the function call timing out before it is actually able to save to the flash
                 *
                 * @return true if it was saved successfully, false otherwise
                 */
                bool savePattern() noexcept;

                /**
                 * Enables the blink1-lib gamma curve
                 *
                 * @note Docs say it should probably always be disabled
                 */
                static void enableDegamma() noexcept;

                /**
                 * Disable the blink1-lib gamma curve
                 *
                 * @note Docs say it should probably always be disabled
                 */
                static void disableDegamma() noexcept;

                /**
                 * Vendor ID for blink1 devices
                 */
                [[nodiscard]] static int vid() noexcept;

                /**
                 * Product ID for blink1 devices
                 */
                [[nodiscard]] static int pid() noexcept;

                /**
                 * Returns the cache index for this device
                 *
                 * @return The cache index if it was found, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<int> getCacheIndex() const noexcept;

                /**
                 * Clear blink1 device cache for this device
                 *
                 * @return The cache index that was cleared if anything was cleared, std::nullopt otherwise
                 */
                std::optional<int> clearCache() noexcept;

                /**
                 * Gets serial ID for this device
                 *
                 * @return 8-hexdigit serial number string if device is good, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<std::string_view> getSerial() const noexcept;

                /**
                 * Returns whether the device is a MK2 device
                 *
                 * @return Whether the device is a MK2 device if the device is good, std::nullopt otherwise
                 */
                [[nodiscard]] std::optional<bool> isMk2() const noexcept;
        };
    }
}
