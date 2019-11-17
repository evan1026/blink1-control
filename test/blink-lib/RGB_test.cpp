#include <sstream>

#include "gtest/gtest.h"
#include "blink-lib/RGB.hpp"

#define SUITE_NAME RGB_test

using namespace blink1_control;

TEST(SUITE_NAME, TestConstructor) {
    RGB rgb1;
    RGB rgb2(123, 231, 144);

    EXPECT_EQ(0, rgb1.r);
    EXPECT_EQ(0, rgb1.g);
    EXPECT_EQ(0, rgb1.b);
    EXPECT_EQ(123, rgb2.r);
    EXPECT_EQ(231, rgb2.g);
    EXPECT_EQ(144, rgb2.b);
}

TEST(SUITE_NAME, TestEqual) {
    RGB rgb(222, 111, 0);
    RGB equalRGB(222, 111, 0);
    RGB notEqualRGB1(123, 111, 0);
    RGB notEqualRGB2(222, 112, 0);
    RGB notEqualRGB3(222, 111, 3);

    EXPECT_EQ(rgb, equalRGB);
    EXPECT_NE(rgb, notEqualRGB1);
    EXPECT_NE(rgb, notEqualRGB2);
    EXPECT_NE(rgb, notEqualRGB3);
}

TEST(SUITE_NAME, TestOutputOperator) {
    std::stringstream ss;
    RGB rgb(5, 6, 7);

    ss << rgb;

    EXPECT_EQ("RGB{r=5, g=6, b=7}", ss.str());
}
