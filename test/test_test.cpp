#include "gtest/gtest.h"
#include "Blink1Device.hpp"
#include "RGB.hpp"

TEST(TestSuite, TestPass) {
    blink1_control::Blink1Device device;
    blink1_control::RGB rgb;
    device.setRGB(rgb);
    ASSERT_EQ(rgb, device.readRGB(0)());
}
TEST(TestSuite, DISABLED_TEST) {
}
