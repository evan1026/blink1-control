#pragma once

#include <blink1-lib.h>
#include <memory>
#include <functional>
#include <Maybe.h>

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

            Maybe<int> getVersion() const;

            bool fadeToRGB(std::uint16_t fadeMillis, RGB rgb);
            bool fadeToRGBN(std::uint16_t fadeMillis, RGBN rgbn);
            bool setRGB(RGB rgb);
            bool setRGBN(RGBN rgbn);
            Maybe<PatternLine> readRGBWithFade(std::uint8_t ledn) const;
            Maybe<RGB> readRGB(std::uint8_t ledn) const;

            bool play(std::uint8_t pos);
            bool playLoop(std::uint8_t startpos, std::uint8_t endpos, std::uint8_t count);
            bool stop();
            Maybe<PlayState> readPlayState() const;

            bool writePatternLine(PatternLine line, std::uint8_t pos);
            bool writePatternLineN(PatternLineN line, std::uint8_t pos);
            Maybe<PatternLine> readPatternLine(std::uint8_t pos) const;
            Maybe<PatternLineN> readPatternLineN(std::uint8_t pos) const;
            bool savePattern();

            static void enableDegamma();
            static void disableDegamma();

            static int vid();
            static int pid();

            Maybe<int> getCacheIndex() const;
            Maybe<int> clearCache();

            Maybe<std::string> getSerial() const;

            Maybe<bool> isMk2() const;
    };
}
