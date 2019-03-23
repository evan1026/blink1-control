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
            Blink1Device(int id);
            Blink1Device(std::string stringInitializer, STRING_INIT_TYPE initType);

            bool good() const;
            operator bool() const;

            std::optional<int> getVersion() const;

            bool fadeToRGB(std::uint16_t fadeMillis, RGB rgb);
            bool fadeToRGBN(std::uint16_t fadeMillis, RGBN rgbn);
            bool setRGB(RGB rgb);
            bool setRGBN(RGBN rgbn);
            std::optional<PatternLine> readRGBWithFade(std::uint8_t ledn) const;
            std::optional<RGB> readRGB(std::uint8_t ledn) const;

            bool play(std::uint8_t pos);
            bool playLoop(std::uint8_t startpos, std::uint8_t endpos, std::uint8_t count);
            bool stop();
            std::optional<PlayState> readPlayState() const;

            bool writePatternLine(PatternLine line, std::uint8_t pos);
            bool writePatternLineN(PatternLineN line, std::uint8_t pos);
            std::optional<PatternLine> readPatternLine(std::uint8_t pos) const;
            std::optional<PatternLineN> readPatternLineN(std::uint8_t pos) const;
            bool savePattern();

            static void enableDegamma();
            static void disableDegamma();

            static int vid();
            static int pid();

            std::optional<int> getCacheIndex() const;
            std::optional<int> clearCache();

            std::optional<std::string> getSerial() const;

            std::optional<bool> isMk2() const;
    };
}
