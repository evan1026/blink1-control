#pragma once

#include <blink1-lib.h>
#include <memory>
#include <functional>
#include <optional>

#include "RGB.hpp"
#include "RGBN.hpp"
#include "PlayState.hpp"
#include "PatternLine.hpp"
#include "PatternLineN.hpp"

namespace blink1_control {
    class Blink1Device {
        std::unique_ptr<blink1_device, std::function<void(blink1_device*)>> device;

        static void destroyBlinkDevice(blink1_device* device) noexcept;

        public:
            enum class STRING_INIT_TYPE { PATH, SERIAL };

            Blink1Device() noexcept;
            Blink1Device(const std::uint32_t id) noexcept;
            Blink1Device(const std::string& stringInitializer, const STRING_INIT_TYPE initType) noexcept;
            Blink1Device(const char* stringInitializer, const STRING_INIT_TYPE initType) noexcept;

            [[nodiscard]] bool good() const noexcept;
            [[nodiscard]] operator bool() const noexcept;

            [[nodiscard]] std::optional<int> getVersion() const noexcept;

            bool fadeToRGB(const std::uint16_t fadeMillis, const RGB& rgb) noexcept;
            bool fadeToRGBN(const std::uint16_t fadeMillis, const RGBN& rgbn) noexcept;
            bool setRGB(const RGB& rgb) noexcept;
            bool setRGBN(const RGBN& rgbn) noexcept;
            [[nodiscard]] std::optional<PatternLine> readRGBWithFade(const std::uint8_t ledn) const noexcept;
            [[nodiscard]] std::optional<RGB> readRGB(const std::uint8_t ledn) const noexcept;

            bool play(const std::uint8_t pos) noexcept;
            bool playLoop(const std::uint8_t startpos, const std::uint8_t endpos, const std::uint8_t count) noexcept;
            bool stop() noexcept;
            [[nodiscard]] std::optional<PlayState> readPlayState() const noexcept;

            bool writePatternLine(const PatternLine& line, const std::uint8_t pos) noexcept;
            bool writePatternLineN(const PatternLineN& line, const std::uint8_t pos) noexcept;
            [[nodiscard]] std::optional<PatternLine> readPatternLine(const std::uint8_t pos) const noexcept;
            [[nodiscard]] std::optional<PatternLineN> readPatternLineN(const std::uint8_t pos) const noexcept;
            bool savePattern() noexcept;

            static void enableDegamma() noexcept;
            static void disableDegamma() noexcept;

            [[nodiscard]] static int vid() noexcept;
            [[nodiscard]] static int pid() noexcept;

            [[nodiscard]] std::optional<int> getCacheIndex() const noexcept;
            std::optional<int> clearCache() noexcept;

            [[nodiscard]] std::optional<std::string_view> getSerial() const noexcept;

            [[nodiscard]] std::optional<bool> isMk2() const noexcept;
    };
}
