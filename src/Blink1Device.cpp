#include "Blink1Device.hpp"

namespace blink1_control {
    Blink1Device::Blink1Device() : device(blink1_open(), Blink1Device::destroyBlinkDevice) {}

    Blink1Device::Blink1Device(int id) : device(blink1_openById(id), Blink1Device::destroyBlinkDevice) {}

    Blink1Device::Blink1Device(std::string stringInitializer, STRING_INIT_TYPE initType) {
        switch (initType) {
            case STRING_INIT_TYPE::PATH:
                device = std::unique_ptr<blink1_device, std::function<void(blink1_device*)>>(blink1_openByPath(stringInitializer.c_str()), Blink1Device::destroyBlinkDevice);
                break;
            case STRING_INIT_TYPE::SERIAL:
                device = std::unique_ptr<blink1_device, std::function<void(blink1_device*)>>(blink1_openBySerial(stringInitializer.c_str()), Blink1Device::destroyBlinkDevice);
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

    bool Blink1Device::fadeToRGB(std::uint16_t fadeMillis, RGB rgb) {
        if (good()) {
            int retVal = blink1_fadeToRGB(device.get(), fadeMillis, rgb.r, rgb.g, rgb.b);
            return retVal == 0;
        }
        return false;
    }

    bool Blink1Device::fadeToRGBN(std::uint16_t fadeMillis, RGBN rgbn) {
        if (good()) {
            int retVal = blink1_fadeToRGBN(device.get(), fadeMillis, rgbn.r, rgbn.g, rgbn.b, rgbn.n);
            return retVal == 0;
        }
        return false;
    }

    bool Blink1Device::setRGB(RGB rgb) {
        if (good()) {
            int retVal = blink1_setRGB(device.get(), rgb.r, rgb.g, rgb.b);
            return retVal == 0;
        }
        return false;
    }

    bool Blink1Device::setRGBN(RGBN rgbn) {
        return fadeToRGBN(0, rgbn);
    }

    std::optional<PatternLine> Blink1Device::readRGBWithFade(std::uint8_t ledn) const {
        if (good()) {
            PatternLine line;
            int retVal = blink1_readRGB(device.get(), &line.fadeMillis, &line.rgb.r, &line.rgb.g, &line.rgb.b, ledn);
            if (retVal == 0) {
                return line;
            }
        }
        return std::nullopt;
    }

    std::optional<RGB> Blink1Device::readRGB(std::uint8_t ledn) const {
        std::optional<PatternLine> pLine = readRGBWithFade(ledn);
        if (pLine) {
            return pLine->rgb;
        }
        return std::nullopt;
    }

    bool Blink1Device::play(std::uint8_t pos) {
        if (good()) {
            int retVal = blink1_play(device.get(), 1, pos);
            return retVal == 0;
        }
        return false;
    }

    bool Blink1Device::playLoop(std::uint8_t startpos, std::uint8_t endpos, std::uint8_t count) {
        if (good()) {
            int retVal = blink1_playloop(device.get(), 1, startpos, endpos, count);
            return retVal == 0;
        }
        return false;
    }

    bool Blink1Device::stop() {
        if (good()) {
            int retVal = blink1_play(device.get(), 0, 0);
            return retVal == 0;
        }
        return false;
    }

    std::optional<PlayState> Blink1Device::readPlayState() const {
        if (good()) {
            PlayState state;
            std::uint8_t playing;
            int retVal = blink1_readPlayState(device.get(), &playing, &state.playStart, &state.playEnd, &state.playCount, &state.playPos);
            state.playing = (playing == 1);
            if (retVal == 0) {
                return state;
            }
        }
        return std::nullopt;
    }

    bool Blink1Device::writePatternLine(PatternLine line, std::uint8_t pos) {
        if (good()) {
            int retVal = blink1_writePatternLine(device.get(), line.fadeMillis, line.rgb.r, line.rgb.g, line.rgb.b, pos);
            return retVal == 0;
        }
        return false;
    }

    bool Blink1Device::writePatternLineN(PatternLineN line, std::uint8_t pos) {
        if (good()) {
            int retVal1 = blink1_setLEDN(device.get(), line.rgbn.n);
            int retVal2 = blink1_writePatternLine(device.get(), line.fadeMillis, line.rgbn.r, line.rgbn.g, line.rgbn.b, pos);
            return retVal1 == 0 && retVal2 == 0;
        }
        return false;
    }

    std::optional<PatternLine> Blink1Device::readPatternLine(std::uint8_t pos) const {
        if (good()) {
            PatternLine line;
            int retVal = blink1_readPatternLine(device.get(), &line.fadeMillis, &line.rgb.r, &line.rgb.g, &line.rgb.b, pos);
            if (retVal == 0) {
                return line;
            }
        }
        return std::nullopt;
    }

    std::optional<PatternLineN> Blink1Device::readPatternLineN(std::uint8_t pos) const {
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
            int retVal = blink1_savePattern(device.get());
            return retVal == 0;
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

    std::optional<std::string> Blink1Device::getSerial() const {
        if (good()) {
            const char* serial = blink1_getSerialForDev(device.get());
            if (serial != nullptr) {
                return std::string(serial);
            }
        }
        return std::nullopt;
    }

    std::optional<bool> Blink1Device::isMk2() const {
        if (good()) {
            int deviceIsMk2 = blink1_isMk2(device.get());
            return deviceIsMk2 == 1;
        }
        return std::nullopt;
    }
}
