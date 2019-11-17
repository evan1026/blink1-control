#include <sstream>

#include "gtest/gtest.h"
#include "blink-lib/RGBN.hpp"

#define SUITE_NAME RGBN_test

using namespace blink1_control;

TEST(SUITE_NAME, TestConstructor) {
    RGBN rgbn1;
    RGBN rgbn2(123, 231, 144, 255);

    EXPECT_EQ(0, rgbn1.r);
    EXPECT_EQ(0, rgbn1.g);
    EXPECT_EQ(0, rgbn1.b);
    EXPECT_EQ(0, rgbn1.n);
    EXPECT_EQ(123, rgbn2.r);
    EXPECT_EQ(231, rgbn2.g);
    EXPECT_EQ(144, rgbn2.b);
    EXPECT_EQ(255, rgbn2.n);
}

TEST(SUITE_NAME, TestEqual) {
    RGBN rgbn(222, 111, 0, 25);
    RGBN equalRGBN(222, 111, 0, 25);
    RGBN notEqualRGBN1(123, 111, 0, 25);
    RGBN notEqualRGBN2(222, 112, 0, 25);
    RGBN notEqualRGBN3(222, 111, 3, 25);
    RGBN notEqualRGBN4(222, 111, 0, 18);

    EXPECT_EQ(rgbn, equalRGBN);
    EXPECT_NE(rgbn, notEqualRGBN1);
    EXPECT_NE(rgbn, notEqualRGBN2);
    EXPECT_NE(rgbn, notEqualRGBN3);
    EXPECT_NE(rgbn, notEqualRGBN4);
}

TEST(SUITE_NAME, TestOutputOperator) {
    std::stringstream ss;
    RGBN rgbn(5, 6, 7, 8);

    ss << rgbn;

    EXPECT_EQ("RGBN{r=5, g=6, b=7, n=8}", ss.str());
}
