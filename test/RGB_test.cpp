#include <sstream>

#include "gtest/gtest.h"
#include "RGB.hpp"

#define SUITE_NAME RGB_test

using namespace blink1_control;

TEST(SUITE_NAME, TestConstructor) {
    RGB rgb1;
    RGB rgb2(123, 231, 144);

    ASSERT_EQ(0, rgb1.r);
    ASSERT_EQ(0, rgb1.g);
    ASSERT_EQ(0, rgb1.b);
    ASSERT_EQ(123, rgb2.r);
    ASSERT_EQ(231, rgb2.g);
    ASSERT_EQ(144, rgb2.b);
}

TEST(SUITE_NAME, TestEqual) {
    RGB rgb(222, 111, 0);
    RGB equalRGB(222, 111, 0);
    RGB notEqualRGB1(123, 111, 0);
    RGB notEqualRGB2(222, 112, 0);
    RGB notEqualRGB3(222, 111, 3);

    ASSERT_EQ(rgb, equalRGB);
    ASSERT_NE(rgb, notEqualRGB1);
    ASSERT_NE(rgb, notEqualRGB2);
    ASSERT_NE(rgb, notEqualRGB3);
}

TEST(SUITE_NAME, TestOutputOperator) {
    std::stringstream ss;
    RGB rgb(5, 6, 7);

    ss << rgb;

    ASSERT_EQ("RGB{r=5, g=6, b=7}", ss.str());
}
