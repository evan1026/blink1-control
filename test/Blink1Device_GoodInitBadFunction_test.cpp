#include <sstream>

#include "gtest/gtest.h"
#include "Blink1Device.hpp"
#include "FakeBlink1Lib.hpp"

using namespace blink1_control;

#define SUITE_NAME Blink1Device_GoodInitBadFunction_test

static void checkDevicesFreed() {
    EXPECT_TRUE(fake_blink1_lib::ALL_DEVICES_FREED()) << "Expected all devices to be freed at the end of the test";
}

class SUITE_NAME : public ::testing::Test {
    protected:
        void SetUp() override {
            fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(false);
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
        if (deviceVersion) {
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

        EXPECT_FALSE(ret) << "Expected fadeToRGB to return false";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestFadeToRGBN) {
    {
        Blink1Device device;
        RGBN rgb(10, 11, 12, 20);
        std::uint16_t fadeMillis = 100;
        bool ret = device.fadeToRGBN(fadeMillis, rgb);

        EXPECT_FALSE(ret) << "Expected fadeToRGB to return false";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestSetRGB) {
    {
        Blink1Device device;
        RGB rgb(10, 11, 12);
        bool ret = device.setRGB(rgb);

        EXPECT_FALSE(ret) << "Expected fadeToRGB to return FALSE";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestSetRGBN) {
    {
        Blink1Device device;
        RGBN rgb(10, 11, 12, 20);
        bool ret = device.setRGBN(rgb);

        EXPECT_FALSE(ret) << "Expected fadeToRGB to return false";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadRGBWithFade) {
    {
        Blink1Device device;
        auto actualline = device.readRGBWithFade(20);
        EXPECT_FALSE(actualline) << "Expected actualline not to be present";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadRGB) {
    {
        Blink1Device device;
        auto actualrgb = device.readRGB(20);
        EXPECT_FALSE(actualrgb) << "Expected actualrgb not to be present";
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestPlay) {
    {
        Blink1Device device;
        bool ret = device.play(5);
        EXPECT_FALSE(ret);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestPlayLoop) {
    {
        Blink1Device device;
        bool ret = device.playLoop(5, 6, 7);
        EXPECT_FALSE(ret);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestStop) {
    {
        Blink1Device device;
        bool ret = device.stop();
        EXPECT_FALSE(ret);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadPlayState) {
    {
        Blink1Device device;
        auto actualState = device.readPlayState();
        EXPECT_FALSE(actualState);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestWritePatternLine) {
    {
        Blink1Device device;

        PatternLine line(1, 2, 3, 4);
        bool ret = device.writePatternLine(line, 20);
        EXPECT_FALSE(ret);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestWritePatternLineN) {
    {
        Blink1Device device;

        PatternLineN line(1, 2, 3, 4, 5);
        bool ret = device.writePatternLineN(line, 20);
        EXPECT_FALSE(ret);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadPatternLine) {
    {
        Blink1Device device;
        auto actualLine = device.readPatternLine(20);
        EXPECT_FALSE(actualLine);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestReadPatternLineN) {
    {
        Blink1Device device;
        auto actualLine = device.readPatternLineN(20);
        EXPECT_FALSE(actualLine);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestSavePattern) {
    {
        Blink1Device device;
        EXPECT_FALSE(device.savePattern());
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
        auto actualIndex = device.getCacheIndex();
        EXPECT_FALSE(actualIndex);
    }
    checkDevicesFreed();
}

TEST_F(SUITE_NAME, TestClearCache) {
    {
        Blink1Device device;
        auto actualIndex = device.clearCache();
        EXPECT_FALSE(actualIndex);
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
            EXPECT_EQ("", *actualSerial);
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
            EXPECT_FALSE(*actualMk2);
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
            EXPECT_FALSE(*actualMk2);
        }
    }
    checkDevicesFreed();
}
