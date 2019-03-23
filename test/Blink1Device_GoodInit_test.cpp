#include <sstream>

#include "gtest/gtest.h"
#include "Blink1Device.hpp"
#include "FakeBlink1Lib.hpp"

using namespace blink1_control;

#define SUITE_NAME Blink1Device_Good_test

static void checkDevicesFreed() {
    EXPECT_TRUE(fake_blink1_lib::ALL_DEVICES_FREED()) << "Expected all devices to be freed at the end of the test";
}

class SUITE_NAME : public ::testing::Test {
    protected:
        void SetUp() override {
            fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(true);
            fake_blink1_lib::SET_BLINK1_SUCCESSFUL_INIT(true);
        }

        void TearDown() override {
            fake_blink1_lib::CLEAR_ALL();
        }
};

TEST_F(SUITE_NAME, TestGood) {
    {
        Blink1Device device;
        EXPECT_TRUE(device.good()) << "Expected device.good() to return true";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestOperatorBool) {
    {
        Blink1Device device;
        EXPECT_TRUE(bool(device)) << "Expected bool(device) to be true";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestGetVersion) {
    {
        Blink1Device device;
        fake_blink1_lib::SET_BLINK1_VERSION(1337);
        auto deviceVersion = device.getVersion();
        EXPECT_TRUE(deviceVersion) << "Expected device version to be present";
        if (deviceVersion) { // Avoid throwing exception if it's not present
            EXPECT_EQ(1337, *deviceVersion);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestFadeToRGB) {
    {
        Blink1Device device;
        RGB rgb(10, 11, 12);
        std::uint16_t fadeMillis = 100;
        bool ret = device.fadeToRGB(fadeMillis, rgb);

        RGB actualRgb = fake_blink1_lib::GET_RGB(0);
        std::uint16_t actualFadeMillis = fake_blink1_lib::GET_FADE_MILLIS(0);

        EXPECT_TRUE(ret) << "Expected fadeToRGB to return true";
        EXPECT_EQ(fadeMillis, actualFadeMillis);
        EXPECT_EQ(rgb, actualRgb);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestFadeToRGBN) {
    {
        Blink1Device device;
        RGBN rgb(10, 11, 12, 20);
        std::uint16_t fadeMillis = 100;
        bool ret = device.fadeToRGBN(fadeMillis, rgb);

        RGB actualRgb = fake_blink1_lib::GET_RGB(20);
        RGB expectRgb(rgb.r, rgb.g, rgb.b);
        std::uint16_t actualFadeMillis = fake_blink1_lib::GET_FADE_MILLIS(20);

        EXPECT_TRUE(ret) << "Expected fadeToRGB to return true";
        EXPECT_EQ(fadeMillis, actualFadeMillis);
        EXPECT_EQ(expectRgb, actualRgb);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestSetRGB) {
    {
        Blink1Device device;
        RGB rgb(10, 11, 12);
        bool ret = device.setRGB(rgb);

        RGB actualRgb = fake_blink1_lib::GET_RGB(0);

        EXPECT_TRUE(ret) << "Expected fadeToRGB to return true";
        EXPECT_EQ(rgb, actualRgb);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestSetRGBN) {
    {
        Blink1Device device;
        RGBN rgb(10, 11, 12, 20);
        bool ret = device.setRGBN(rgb);

        RGB actualRgb = fake_blink1_lib::GET_RGB(20);
        RGB expectRgb(rgb.r, rgb.g, rgb.b);

        EXPECT_TRUE(ret) << "Expected fadeToRGB to return true";
        EXPECT_EQ(expectRgb, actualRgb);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadRGBWithFade) {
    {
        Blink1Device device;
        RGB rgb(10, 11, 12);
        std::uint16_t fadeMillis = 97;

        fake_blink1_lib::SET_RGB(rgb, 20);
        fake_blink1_lib::SET_FADE_MILLIS(fadeMillis, 20);

        auto actualline = device.readRGBWithFade(20);
        PatternLine expectLine(rgb.r, rgb.g, rgb.b, fadeMillis);

        EXPECT_TRUE(actualline) << "Expected actualline to be present";
        if (actualline) {
            EXPECT_EQ(expectLine, *actualline);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadRGB) {
    {
        Blink1Device device;
        RGB rgb(10, 11, 12);
        std::uint16_t fadeMillis = 97;

        fake_blink1_lib::SET_RGB(rgb, 20);
        fake_blink1_lib::SET_FADE_MILLIS(fadeMillis, 20); // underlying blink1-lib function reads millis as well

        auto actualrgb = device.readRGB(20);

        EXPECT_TRUE(actualrgb) << "Expected actualrgb to be present";
        if (actualrgb) {
            EXPECT_EQ(rgb, *actualrgb);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestPlay) {
    {
        Blink1Device device;
        bool ret = device.play(5);

        PlayState state = fake_blink1_lib::GET_PLAY_STATE();
        EXPECT_TRUE(ret);
        EXPECT_TRUE(state.playing);
        EXPECT_EQ(5, state.playPos);
        EXPECT_EQ(5, state.playEnd); // Probably not the case for real but correct based on fake lib
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestPlayLoop) {
    {
        Blink1Device device;
        bool ret = device.playLoop(5, 6, 7);

        PlayState state = fake_blink1_lib::GET_PLAY_STATE();
        EXPECT_TRUE(ret);
        EXPECT_TRUE(state.playing);
        EXPECT_EQ(5, state.playStart);
        EXPECT_EQ(6, state.playEnd);
        EXPECT_EQ(7, state.playCount);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestStop) {
    {
        Blink1Device device;

        PlayState state;
        state.playing = true;
        fake_blink1_lib::SET_PLAY_STATE(state);

        bool ret = device.stop();
        state = fake_blink1_lib::GET_PLAY_STATE();

        EXPECT_TRUE(ret);
        EXPECT_FALSE(state.playing);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadPlayState) {
    {
        Blink1Device device;

        PlayState state(true, 2, 3, 4, 5);
        fake_blink1_lib::SET_PLAY_STATE(state);

        auto actualState = device.readPlayState();

        EXPECT_TRUE(actualState);
        if (actualState) {
            EXPECT_TRUE(actualState->playing);
            EXPECT_EQ(2, actualState->playStart);
            EXPECT_EQ(3, actualState->playEnd);
            EXPECT_EQ(4, actualState->playCount);
            EXPECT_EQ(5, actualState->playPos);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestWritePatternLine) {
    {
        Blink1Device device;

        PatternLine line(1, 2, 3, 4);
        bool ret = device.writePatternLine(line, 20);

        PatternLineN actualLine = fake_blink1_lib::GET_PATTERN_LINE(20);

        EXPECT_TRUE(ret);
        EXPECT_EQ(1, actualLine.rgbn.r);
        EXPECT_EQ(2, actualLine.rgbn.g);
        EXPECT_EQ(3, actualLine.rgbn.b);
        EXPECT_EQ(4, actualLine.fadeMillis);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestWritePatternLineN) {
    {
        Blink1Device device;

        PatternLineN line(1, 2, 3, 4, 5);
        bool ret = device.writePatternLineN(line, 20);

        PatternLineN actualLine = fake_blink1_lib::GET_PATTERN_LINE(20);

        EXPECT_TRUE(ret);
        EXPECT_EQ(1, actualLine.rgbn.r);
        EXPECT_EQ(2, actualLine.rgbn.g);
        EXPECT_EQ(3, actualLine.rgbn.b);
        EXPECT_EQ(4, actualLine.rgbn.n);
        EXPECT_EQ(5, actualLine.fadeMillis);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadPatternLine) {
    {
        Blink1Device device;

        PatternLineN line(1, 2, 3, 4, 5);
        fake_blink1_lib::SET_PATTERN_LINE(line, 20);

        auto actualLine = device.readPatternLine(20);
        EXPECT_TRUE(actualLine);
        if (actualLine) {
            EXPECT_EQ(1, actualLine->rgb.r);
            EXPECT_EQ(2, actualLine->rgb.g);
            EXPECT_EQ(3, actualLine->rgb.b);
            EXPECT_EQ(5, actualLine->fadeMillis);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadPatternLineN) {
    {
        Blink1Device device;

        PatternLineN line(1, 2, 3, 4, 5);
        fake_blink1_lib::SET_PATTERN_LINE(line, 20);

        auto actualLine = device.readPatternLineN(20);
        EXPECT_TRUE(actualLine);
        if (actualLine) {
            EXPECT_EQ(1, actualLine->rgbn.r);
            EXPECT_EQ(2, actualLine->rgbn.g);
            EXPECT_EQ(3, actualLine->rgbn.b);
            EXPECT_EQ(4, actualLine->rgbn.n);
            EXPECT_EQ(5, actualLine->fadeMillis);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestSavePattern) {
    {
        Blink1Device device;
        EXPECT_TRUE(device.savePattern());
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestDegamma) {
    {
        Blink1Device::enableDegamma();
        EXPECT_TRUE(fake_blink1_lib::degammaEnabled);

        Blink1Device::disableDegamma();
        EXPECT_FALSE(fake_blink1_lib::degammaEnabled);

        Blink1Device::enableDegamma();
        EXPECT_TRUE(fake_blink1_lib::degammaEnabled);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestVid) {
    {
        fake_blink1_lib::SET_BLINK1_VID(12345);
        EXPECT_EQ(12345, Blink1Device::vid());
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestPid) {
    {
        fake_blink1_lib::SET_BLINK1_PID(12345);
        EXPECT_EQ(12345, Blink1Device::pid());
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestGetCacheIndex) {
    {
        Blink1Device device;

        fake_blink1_lib::SET_CACHE_INDEX(98);
        auto actualIndex = device.getCacheIndex();

        EXPECT_TRUE(actualIndex);
        if (actualIndex) {
            EXPECT_EQ(98, *actualIndex);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestClearCache) {
    {
        Blink1Device device;

        fake_blink1_lib::SET_CACHE_INDEX(98);
        auto actualIndex = device.clearCache();

        EXPECT_TRUE(actualIndex);
        if (actualIndex) {
            EXPECT_EQ(98, *actualIndex);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestGetSerial) {
    {
        Blink1Device device;

        fake_blink1_lib::SET_SERIAL("SERIAL");
        auto actualSerial = device.getSerial();

        EXPECT_TRUE(actualSerial);
        if (actualSerial) {
            EXPECT_EQ("SERIAL", *actualSerial);
        }
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestIsMk2) {
    {
        Blink1Device device;

        fake_blink1_lib::SET_IS_MK2(true);
        auto actualMk2 = device.isMk2();
        EXPECT_TRUE(actualMk2);
        if (actualMk2) {
            EXPECT_TRUE(*actualMk2);
        }

        fake_blink1_lib::SET_IS_MK2(false);
        actualMk2 = device.isMk2();
        EXPECT_TRUE(actualMk2);
        if (actualMk2) {
            EXPECT_FALSE(*actualMk2);
        }

        fake_blink1_lib::SET_IS_MK2(true);
        actualMk2 = device.isMk2();
        EXPECT_TRUE(actualMk2);
        if (actualMk2) {
            EXPECT_TRUE(*actualMk2);
        }
    }
    checkDevicesFreed();
}
