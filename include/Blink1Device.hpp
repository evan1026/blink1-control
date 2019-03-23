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

        static void destroyBlinkDevice(blink1_device* device);

        public:
            enum class STRING_INIT_TYPE { PATH, SERIAL };

            Blink1Device();
            Blink1Device(const int id);
            Blink1Device(const std::string& stringInitializer, const STRING_INIT_TYPE initType);
            Blink1Device(const char* stringInitializer, const STRING_INIT_TYPE initType);

            bool good() const;
            operator bool() const;

            std::optional<int> getVersion() const;

            bool fadeToRGB(const std::uint16_t fadeMillis, const RGB& rgb);
            bool fadeToRGBN(const std::uint16_t fadeMillis, const RGBN& rgbn);
            bool setRGB(const RGB& rgb);
            bool setRGBN(const RGBN& rgbn);
            std::optional<PatternLine> readRGBWithFade(const std::uint8_t ledn) const;
            std::optional<RGB> readRGB(const std::uint8_t ledn) const;

            bool play(const std::uint8_t pos);
            bool playLoop(const std::uint8_t startpos, const std::uint8_t endpos, const std::uint8_t count);
            bool stop();
            std::optional<PlayState> readPlayState() const;

            bool writePatternLine(const PatternLine& line, const std::uint8_t pos);
            bool writePatternLineN(const PatternLineN& line, const std::uint8_t pos);
            std::optional<PatternLine> readPatternLine(const std::uint8_t pos) const;
            std::optional<PatternLineN> readPatternLineN(const std::uint8_t pos) const;
            bool savePattern();

            static void enableDegamma();
            static void disableDegamma();

            static int vid();
            static int pid();

            std::optional<int> getCacheIndex() const;
            std::optional<int> clearCache();

            std::optional<std::string_view> getSerial() const;

            std::optional<bool> isMk2() const;
    };
}
