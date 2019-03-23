#include "Blink1Device.hpp"

namespace blink1_control {
    Blink1Device::Blink1Device() : device(blink1_open(), Blink1Device::destroyBlinkDevice) {}

    Blink1Device::Blink1Device(int id) : device(blink1_openById(id), Blink1Device::destroyBlinkDevice) {}

    Blink1Device::Blink1Device(const std::string& stringInitializer, STRING_INIT_TYPE initType) : Blink1Device(stringInitializer.c_str(), initType) {}

    Blink1Device::Blink1Device(const char* stringInitializer, STRING_INIT_TYPE initType) {
        switch (initType) {
            case STRING_INIT_TYPE::PATH:
                device = std::unique_ptr<blink1_device, std::function<void(blink1_device*)>>(blink1_openByPath(stringInitializer), Blink1Device::destroyBlinkDevice);
                break;
            case STRING_INIT_TYPE::SERIAL:
                device = std::unique_ptr<blink1_device, std::function<void(blink1_device*)>>(blink1_openBySerial(stringInitializer), Blink1Device::destroyBlinkDevice);
                break;
            default:
                device = nullptr;
                break;
        }
    }

    void Blink1Device::destroyBlinkDevice(blink1_device* device) {
        blink1_close(device);
    }

    bool Blink1Device::good() const {
        return device != nullptr;
    }

    Blink1Device::operator bool() const {
        return good();
    }

    std::optional<int> Blink1Device::getVersion() const {
        if (good()) {
            return blink1_getVersion(device.get());
        }
        return std::nullopt;
    }

    bool Blink1Device::fadeToRGB(const std::uint16_t fadeMillis, const RGB& rgb) {
        if (good()) {
            return 0 == blink1_fadeToRGB(device.get(), fadeMillis, rgb.r, rgb.g, rgb.b);
        }
        return false;
    }

    bool Blink1Device::fadeToRGBN(const std::uint16_t fadeMillis, const RGBN& rgbn) {
        if (good()) {
            return 0 == blink1_fadeToRGBN(device.get(), fadeMillis, rgbn.r, rgbn.g, rgbn.b, rgbn.n);
        }
        return false;
    }

    bool Blink1Device::setRGB(const RGB& rgb) {
        if (good()) {
            return 0 == blink1_setRGB(device.get(), rgb.r, rgb.g, rgb.b);
        }
        return false;
    }

    bool Blink1Device::setRGBN(const RGBN& rgbn) {
        return fadeToRGBN(0, rgbn);
    }

    std::optional<PatternLine> Blink1Device::readRGBWithFade(const std::uint8_t ledn) const {
        if (good()) {
            PatternLine line;
            const auto retVal = blink1_readRGB(device.get(), &line.fadeMillis, &line.rgb.r, &line.rgb.g, &line.rgb.b, ledn);
            if (retVal == 0) {
                return line;
            }
        }
        return std::nullopt;
    }

    std::optional<RGB> Blink1Device::readRGB(const std::uint8_t ledn) const {
        const auto pLine = readRGBWithFade(ledn);
        if (pLine) {
            return pLine->rgb;
        }
        return std::nullopt;
    }

    bool Blink1Device::play(const std::uint8_t pos) {
        if (good()) {
            return 0 == blink1_play(device.get(), 1, pos);
        }
        return false;
    }

    bool Blink1Device::playLoop(std::uint8_t startpos, std::uint8_t endpos, std::uint8_t count) {
        if (good()) {
            return 0 == blink1_playloop(device.get(), 1, startpos, endpos, count);
        }
        return false;
    }

    bool Blink1Device::stop() {
        if (good()) {
            return 0 == blink1_play(device.get(), 0, 0);
        }
        return false;
    }

    std::optional<PlayState> Blink1Device::readPlayState() const {
        if (good()) {
            PlayState state;
            std::uint8_t playing;
            const auto retVal = blink1_readPlayState(device.get(), &playing, &state.playStart, &state.playEnd, &state.playCount, &state.playPos);
            state.playing = (playing == 1);
            if (retVal == 0) {
                return state;
            }
        }
        return std::nullopt;
    }

    bool Blink1Device::writePatternLine(const PatternLine& line, const std::uint8_t pos) {
        if (good()) {
            return 0 == blink1_writePatternLine(device.get(), line.fadeMillis, line.rgb.r, line.rgb.g, line.rgb.b, pos);
        }
        return false;
    }

    bool Blink1Device::writePatternLineN(const PatternLineN& line, const std::uint8_t pos) {
        if (good()) {
            const auto retVal1 = blink1_setLEDN(device.get(), line.rgbn.n);
            const auto retVal2 = blink1_writePatternLine(device.get(), line.fadeMillis, line.rgbn.r, line.rgbn.g, line.rgbn.b, pos);
            return retVal1 == 0 && retVal2 == 0;
        }
        return false;
    }

    std::optional<PatternLine> Blink1Device::readPatternLine(const std::uint8_t pos) const {
        if (good()) {
            PatternLine line;
            int retVal = blink1_readPatternLine(device.get(), &line.fadeMillis, &line.rgb.r, &line.rgb.g, &line.rgb.b, pos);
            if (retVal == 0) {
                return line;
            }
        }
        return std::nullopt;
    }

    std::optional<PatternLineN> Blink1Device::readPatternLineN(const std::uint8_t pos) const {
        if (good()) {
            PatternLineN line;
            int retVal = blink1_readPatternLineN(device.get(), &line.fadeMillis, &line.rgbn.r, &line.rgbn.g, &line.rgbn.b, &line.rgbn.n, pos);
            if (retVal == 0) {
                return line;
            }
        }
        return std::nullopt;
    }

    bool Blink1Device::savePattern() {
        if (good()) {
            return 0 == blink1_savePattern(device.get());
        }
        return false;
    }

    void Blink1Device::enableDegamma() {
        blink1_enableDegamma();
    }

    void Blink1Device::disableDegamma() {
        blink1_disableDegamma();
    }

    int Blink1Device::vid() {
        return blink1_vid();
    }

    int Blink1Device::pid() {
        return blink1_pid();
    }

    std::optional<int> Blink1Device::getCacheIndex() const {
        if (good()) {
            int cacheIndex = blink1_getCacheIndexByDev(device.get());
            if (cacheIndex != -1) {
                return cacheIndex;
            }
        }
        return std::nullopt;
    }

    std::optional<int> Blink1Device::clearCache() {
        if (good()) {
            int cacheIndex = blink1_clearCacheDev(device.get());
            if (cacheIndex != -1) {
                return cacheIndex;
            }
        }
        return std::nullopt;
    }

    std::optional<std::string_view> Blink1Device::getSerial() const {
        if (good()) {
            const char* serial = blink1_getSerialForDev(device.get());
            if (serial != nullptr) {
                return serial;
            }
        }
        return std::nullopt;
    }

    std::optional<bool> Blink1Device::isMk2() const {
        if (good()) {
            return 1 == blink1_isMk2(device.get());
        }
        return std::nullopt;
    }
}
