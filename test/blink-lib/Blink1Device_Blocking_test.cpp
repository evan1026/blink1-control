#include <chrono>

#include "gtest/gtest.h"
#include "blink-lib/Blink1Device.hpp"
#include "FakeBlink1Lib.hpp"

using namespace blink1_control::blink1_lib;

#define SUITE_NAME Blink1Device_Blocking_test

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

TEST_F(SUITE_NAME, TestGetterSetter) {
    Blink1Device device;
    EXPECT_FALSE(device.isBlocking()) << "Expected device to be non-blocking by default";

    device.setBlocking(true);
    EXPECT_TRUE(device.isBlocking()) << "Expected device.setBlocking(true) to set device to blocking";

    device.setBlocking(false);
    EXPECT_FALSE(device.isBlocking()) << "Expected device.setBlocking(false) to set device to non-blocking";

    device.setBlocking();
    EXPECT_TRUE(device.isBlocking()) << "Expected device.setBlocking() to set device to blocking";

    device.setNonBlocking();
    EXPECT_FALSE(device.isBlocking()) << "Expected device.setNonBlocking() to set device to non-blocking";
}

TEST_F(SUITE_NAME, TestBlocking) {
    RGB rgb(10, 11, 12);
    RGBN rgbn(10, 11, 12, 20);
    std::uint16_t fadeMillis = 100;

    Blink1Device device;
    device.setBlocking();

    auto currentTime = std::chrono::system_clock::now();
    device.fadeToRGB(fadeMillis, rgb);
    auto timeAfterFadeRgb = std::chrono::system_clock::now();
    EXPECT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFadeRgb - currentTime).count(), 100) << "Expected fadeToRGB to take at least 100ms";

    currentTime = std::chrono::system_clock::now();
    device.fadeToRGBN(fadeMillis, rgbn);
    auto timeAfterFadeRgbn = std::chrono::system_clock::now();
    EXPECT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFadeRgbn - currentTime).count(), 100) << "Expected fadeToRGBN to take at least 100ms";
}

TEST_F(SUITE_NAME, TestNonBlocking) {
    RGB rgb(10, 11, 12);
    RGBN rgbn(10, 11, 12, 20);
    std::uint16_t fadeMillis = 100;

    Blink1Device device;
    device.setNonBlocking();

    auto currentTime = std::chrono::system_clock::now();
    device.fadeToRGB(fadeMillis, rgb);
    auto timeAfterFadeRgb = std::chrono::system_clock::now();
    EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFadeRgb - currentTime).count(), 10) << "Expected fadeToRGB to take less than 10ms";

    currentTime = std::chrono::system_clock::now();
    device.fadeToRGBN(fadeMillis, rgbn);
    auto timeAfterFadeRgbn = std::chrono::system_clock::now();
    EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFadeRgbn - currentTime).count(), 10) << "Expected fadeToRGBN to take less than 10ms";
}

TEST_F(SUITE_NAME, TestNonBlockingOnFailure) {
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(false);

    RGB rgb(10, 11, 12);
    RGBN rgbn(10, 11, 12, 20);
    std::uint16_t fadeMillis = 100;

    Blink1Device device;
    device.setBlocking();

    auto currentTime = std::chrono::system_clock::now();
    device.fadeToRGB(fadeMillis, rgb);
    auto timeAfterFadeRgb = std::chrono::system_clock::now();
    EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFadeRgb - currentTime).count(), 10) << "Expected fadeToRGB to take less than 10ms";

    currentTime = std::chrono::system_clock::now();
    device.fadeToRGBN(fadeMillis, rgbn);
    auto timeAfterFadeRgbn = std::chrono::system_clock::now();
    EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFadeRgbn - currentTime).count(), 10) << "Expected fadeToRGBN to take less than 10ms";
}
