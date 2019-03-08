#include <vector>
#include <algorithm>
#include <map>

#include "gtest/gtest.h"
#include "blink1-lib.h"

#include "RGB.hpp"
#include "PlayState.hpp"
#include "PatternLineN.hpp"
#include "FakeBlink1Lib.hpp"

using namespace blink1_control;

std::vector<blink1_device*> fake_blink1_lib::blink1_devices;
std::map<long, blink1_control::RGB> fake_blink1_lib::ledColors;
std::map<long, uint16_t> fake_blink1_lib::ledFadeMillis;
std::map<long, blink1_control::PatternLineN> fake_blink1_lib::patternLines;
int fake_blink1_lib::cacheIndex = 0;
std::string fake_blink1_lib::serial;
bool fake_blink1_lib::isMk2;
int fake_blink1_lib::patternLineLEDN = 0;
blink1_control::PlayState fake_blink1_lib::playState;
int fake_blink1_lib::blink1Version = 0;
bool fake_blink1_lib::successfulOperation = false;
bool fake_blink1_lib::successfulInit = false;
bool fake_blink1_lib::degammaEnabled = false;
int fake_blink1_lib::vid = 0;
int fake_blink1_lib::pid = 0;
/*********************
 * METHODS FOR TESTS *
 *********************/

void fake_blink1_lib::CLEAR_ALL() {
    for (auto it = blink1_devices.begin(); it != blink1_devices.end(); ++it) {
        blink1_device* device = *it;
        it = blink1_devices.erase(it);
        delete device;
    }

    ledColors.clear();
    ledFadeMillis.clear();
    patternLines.clear();

    cacheIndex = 0;
    serial = "";
    isMk2 = false;
    patternLineLEDN = 0;
    playState = blink1_control::PlayState();
    blink1Version = 0;
    successfulOperation = false;
    successfulInit = false;
    degammaEnabled = false;
    vid = 0;
    pid = 0;
}

bool fake_blink1_lib::ALL_DEVICES_FREED() {
    return blink1_devices.size() == 0;
}

void fake_blink1_lib::SET_BLINK1_VERSION(int version) {
    blink1Version = version;
}

void fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(bool op) {
    successfulOperation = op;
}

void fake_blink1_lib::SET_BLINK1_SUCCESSFUL_INIT(bool init) {
    successfulInit = init;
}

void fake_blink1_lib::SET_BLINK1_VID(int _vid) {
    vid = _vid;
}

void fake_blink1_lib::SET_BLINK1_PID(int _pid) {
    pid = _pid;
}

void fake_blink1_lib::SET_CACHE_INDEX(int index) {
    cacheIndex = index;
}

void fake_blink1_lib::SET_SERIAL(std::string _serial) {
    serial = _serial;
}

void fake_blink1_lib::SET_IS_MK2(bool mk2) {
    isMk2 = mk2;
}

bool fake_blink1_lib::SUCCESS(blink1_device* dev) {
    return successfulOperation && dev != nullptr;
}

RGB fake_blink1_lib::GET_RGB(long n) {
    if (ledColors.find(n) == ledColors.end()) {
        ADD_FAILURE() << "LED color " << n << " has not yet been initialized.";
        return RGB();
    }
    return ledColors.at(n);
}

void fake_blink1_lib::SET_RGB(blink1_control::RGB rgb, long n) {
    ledColors[n] = rgb;
}

long fake_blink1_lib::GET_FADE_MILLIS(long n) {
    if (ledFadeMillis.find(n) == ledFadeMillis.end()) {
        ADD_FAILURE() << "LED fade millis " << n << " has not yet been initialized.";
        return -1;
    }
    return ledFadeMillis.at(n);
}

void fake_blink1_lib::SET_FADE_MILLIS(long fadeMillis, long n) {
    ledFadeMillis[n] = fadeMillis;
}

PatternLineN fake_blink1_lib::GET_PATTERN_LINE(long pos) {
    if (patternLines.find(pos) == patternLines.end()) {
        EXPECT_TRUE(false) << "Pattern Line " << pos << " has not yet been initialized.";
        return PatternLineN();
    }
    return patternLines.at(pos);
}

void fake_blink1_lib::SET_PATTERN_LINE(blink1_control::PatternLineN line, long pos) {
    patternLines[pos] = line;
}

PlayState fake_blink1_lib::GET_PLAY_STATE() {
    return playState;
}

void fake_blink1_lib::SET_PLAY_STATE(PlayState state) {
    playState = state;
}

/******************
 * MOCKED METHODS *
 ******************/

blink1_device* blink1_open() {
    if (fake_blink1_lib::successfulInit) {
        blink1_device* newDevice = new blink1_device();
        fake_blink1_lib::blink1_devices.push_back(newDevice);
        return newDevice;
    } else {
        return nullptr;
    }
}

blink1_device* blink1_openByPath(const char* path) {
    return blink1_open();
}

blink1_device* blink1_openBySerial(const char* serial) {
    return blink1_open();
}

blink1_device* blink1_openById(uint32_t id) {
    return blink1_open();
}

void blink1_close(blink1_device* dev) {
    auto loc = std::find(fake_blink1_lib::blink1_devices.begin(), fake_blink1_lib::blink1_devices.end(), dev);
    if (loc != fake_blink1_lib::blink1_devices.end()) {
        fake_blink1_lib::blink1_devices.erase(loc);
    } else {
        ADD_FAILURE() << "Tried to delete device that was never allocated: " << dev;
    }

    delete dev;
}

int blink1_getVersion(blink1_device* dev) {
    auto loc = std::find(fake_blink1_lib::blink1_devices.begin(), fake_blink1_lib::blink1_devices.end(), dev);
    if (loc != fake_blink1_lib::blink1_devices.end()) {
        return fake_blink1_lib::blink1Version;
    } else {
        //TODO docs don't specify failure case, just that device must be initialized
        ADD_FAILURE() << "Getting version from uninitialized device.";
        return -1;
    }
}

// This does LED 0 which actually sets all LEDs
// Does LED 0 first to make sure that it gets created in the map
int blink1_fadeToRGB(blink1_device* dev, uint16_t fadeMillis, uint8_t r, uint8_t g, uint8_t b) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::ledFadeMillis[0] = fadeMillis;
        fake_blink1_lib::ledColors[0] = RGB(r, g, b);

        for (auto i = fake_blink1_lib::ledFadeMillis.begin(); i != fake_blink1_lib::ledFadeMillis.end(); ++i) {
            i->second = fadeMillis;
        }
        for (auto i = fake_blink1_lib::ledColors.begin(); i != fake_blink1_lib::ledColors.end(); ++i) {
            i->second = RGB(r, g, b);
        }
        return 0;
    } else {
        return -1;
    }
}

int blink1_fadeToRGBN(blink1_device* dev, uint16_t fadeMillis, uint8_t r, uint8_t g, uint8_t b, uint8_t n) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::ledFadeMillis[n] = fadeMillis;
        fake_blink1_lib::ledColors[n] = RGB(r, g, b);
        return 0;
    } else {
        return -1;
    }
}

int blink1_setRGB(blink1_device* dev, uint8_t r, uint8_t g, uint8_t b) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::ledColors[0] = RGB(r, g, b);
        for (auto i = fake_blink1_lib::ledColors.begin(); i != fake_blink1_lib::ledColors.end(); ++i) {
            i->second = RGB(r, g, b);
        }
        return 0;
    } else {
        return -1;
    }
}

int blink1_readRGB(blink1_device* dev, uint16_t* fadeMillis, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t ledn) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        RGB ledRgb = fake_blink1_lib::GET_RGB(ledn);
        *fadeMillis = fake_blink1_lib::GET_FADE_MILLIS(ledn);
        *r = ledRgb.r;
        *g = ledRgb.g;
        *b = ledRgb.b;
        return 0;
    } else {
        return -1;
    }
}

int blink1_play(blink1_device* dev, uint8_t play, uint8_t pos) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::playState.playing = (play == 1);
        fake_blink1_lib::playState.playPos = pos;
        fake_blink1_lib::playState.playEnd = pos;
        return 0;
    } else {
        return -1;
    }
}

int blink1_playloop(blink1_device* dev, uint8_t play, uint8_t startpos, uint8_t endpos, uint8_t count) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::playState.playing = (play == 1);
        fake_blink1_lib::playState.playStart = startpos;
        fake_blink1_lib::playState.playEnd = endpos;
        fake_blink1_lib::playState.playCount = count;
        return 0;
    } else {
        return -1;
    }
}

int blink1_readPlayState(blink1_device* dev, uint8_t* playing, uint8_t* playstart, uint8_t* playend, uint8_t* playcount, uint8_t* playpos) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        *playing = fake_blink1_lib::playState.playing ? 1 : 0;
        *playstart = fake_blink1_lib::playState.playStart;
        *playend = fake_blink1_lib::playState.playEnd;
        *playcount = fake_blink1_lib::playState.playCount;
        *playpos = fake_blink1_lib::playState.playPos;
        return 0;
    } else {
        return -1;
    }
}

int blink1_writePatternLine(blink1_device* dev, uint16_t fadeMillis, uint8_t r, uint8_t g, uint8_t b, uint8_t pos) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::patternLines[pos] = PatternLineN(r, g, b, fake_blink1_lib::patternLineLEDN, fadeMillis);
        return 0;
    } else {
        return -1;
    }
}

int blink1_readPatternLine(blink1_device* dev, uint16_t* fadeMillis, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t pos) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        PatternLineN line = fake_blink1_lib::GET_PATTERN_LINE(pos);
        *r = line.rgbn.r;
        *g = line.rgbn.g;
        *b = line.rgbn.b;
        *fadeMillis = line.fadeMillis;
        return 0;
    } else {
        return -1;
    }
}

int blink1_readPatternLineN(blink1_device* dev, uint16_t* fadeMillis, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* ledn, uint8_t pos) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        PatternLineN line = fake_blink1_lib::GET_PATTERN_LINE(pos);
        *r = line.rgbn.r;
        *g = line.rgbn.g;
        *b = line.rgbn.b;
        *ledn = line.rgbn.n;
        *fadeMillis = line.fadeMillis;
        return 0;
    } else {
        return -1;
    }
}

int blink1_savePattern(blink1_device* dev) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        return 0;
    } else {
        return -1;
    }
}

int blink1_setLEDN(blink1_device* dev, uint8_t ledn) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        fake_blink1_lib::patternLineLEDN = ledn;
        return 0;
    } else {
        return -1;
    }
}

void blink1_enableDegamma() {
    fake_blink1_lib::degammaEnabled = true;
}

void blink1_disableDegamma() {
    fake_blink1_lib::degammaEnabled = false;
}

int blink1_vid() {
    return fake_blink1_lib::vid;
}

int blink1_pid() {
    return fake_blink1_lib::pid;
}

int blink1_getCacheIndexByDev(blink1_device* dev) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        return fake_blink1_lib::cacheIndex;
    } else {
        return -1;
    }
}

int blink1_clearCacheDev(blink1_device* dev) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        return fake_blink1_lib::cacheIndex;
    } else {
        return -1;
    }
}

const char* blink1_getSerialForDev(blink1_device* dev) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        return fake_blink1_lib::serial.c_str();
    } else {
        // TODO ????? - Docs don't specify what happens if it's invalid
        return "";
    }
}

int blink1_isMk2(blink1_device* dev) {
    if (fake_blink1_lib::SUCCESS(dev)) {
        return fake_blink1_lib::isMk2;
    } else {
        // TODO ????? - Docs don't specify what happens if it's invalid
        return 0;
    }
}
