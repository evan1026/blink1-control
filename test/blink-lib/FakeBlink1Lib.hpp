#pragma once

#include <exception>
#include <string>
#include <vector>
#include <map>

#include "blink-lib/RGB.hpp"
#include "blink-lib/PlayState.hpp"
#include "blink-lib/PatternLineN.hpp"

#include "blink1-lib.h"

typedef struct hid_device_ {} hid_device_;

namespace fake_blink1_lib {
    extern std::vector<blink1_device*> blink1_devices;
    extern std::map<long, blink1_control::blink1_lib::RGB> ledColors;
    extern std::map<long, uint16_t> ledFadeMillis;
    extern std::map<long, blink1_control::blink1_lib::PatternLineN> patternLines;
    extern int cacheIndex;
    extern std::string serial;
    extern bool isMk2;
    extern int patternLineLEDN;
    extern blink1_control::blink1_lib::PlayState playState;
    extern int blink1Version;
    extern bool successfulOperation;
    extern bool successfulInit;
    extern bool degammaEnabled;
    extern int vid;
    extern int pid;

    void CLEAR_ALL();

    bool ALL_DEVICES_FREED();

    void SET_BLINK1_VERSION(int version);

    void SET_BLINK1_SUCCESSFUL_OPERATION(bool op);
    void SET_BLINK1_SUCCESSFUL_INIT(bool init);

    void SET_BLINK1_VID(int _vid);

    void SET_BLINK1_PID(int _pid);

    void SET_CACHE_INDEX(int index);

    void SET_SERIAL(std::string serial);

    void SET_IS_MK2(bool mk2);

    bool SUCCESS(blink1_device* dev);

    blink1_control::blink1_lib::RGB GET_RGB(long n);

    void SET_RGB(blink1_control::blink1_lib::RGB rgb, long n);

    long GET_FADE_MILLIS(long n);

    void SET_FADE_MILLIS(long fadeMillis, long n);

    blink1_control::blink1_lib::PatternLineN GET_PATTERN_LINE(long pos);

    void SET_PATTERN_LINE(blink1_control::blink1_lib::PatternLineN line, long pos);

    blink1_control::blink1_lib::PlayState GET_PLAY_STATE();

    void SET_PLAY_STATE(blink1_control::blink1_lib::PlayState state);
}
