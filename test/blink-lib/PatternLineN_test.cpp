#include <sstream>

#include "gtest/gtest.h"
#include "blink-lib/PatternLineN.hpp"

#define SUITE_NAME PatternLineN_test

using namespace blink1_control::blink1_lib;

TEST(SUITE_NAME, TestConstructor) {
    PatternLineN patternLine1;
    PatternLineN patternLine2(RGBN(1, 2, 3, 4), 5);
    PatternLineN patternLine3(6, 7, 8, 9, 10);

    EXPECT_EQ(0, patternLine1.rgbn.r);
    EXPECT_EQ(0, patternLine1.rgbn.g);
    EXPECT_EQ(0, patternLine1.rgbn.b);
    EXPECT_EQ(0, patternLine1.rgbn.n);
    EXPECT_EQ(0, patternLine1.fadeMillis);

    EXPECT_EQ(1, patternLine2.rgbn.r);
    EXPECT_EQ(2, patternLine2.rgbn.g);
    EXPECT_EQ(3, patternLine2.rgbn.b);
    EXPECT_EQ(4, patternLine2.rgbn.n);
    EXPECT_EQ(5, patternLine2.fadeMillis);

    EXPECT_EQ(6, patternLine3.rgbn.r);
    EXPECT_EQ(7, patternLine3.rgbn.g);
    EXPECT_EQ(8, patternLine3.rgbn.b);
    EXPECT_EQ(9, patternLine3.rgbn.n);
    EXPECT_EQ(10, patternLine3.fadeMillis);
}

TEST(SUITE_NAME, TestEqual) {
    PatternLineN patternLine(1, 2, 3, 4, 5);
    PatternLineN equalPatternLine(1, 2, 3, 4, 5);
    PatternLineN notEqualPatternLine1(2, 2, 3, 4, 5);
    PatternLineN notEqualPatternLine2(1, 3, 3, 4, 5);
    PatternLineN notEqualPatternLine3(1, 2, 4, 4, 5);
    PatternLineN notEqualPatternLine4(1, 2, 3, 5, 5);
    PatternLineN notEqualPatternLine5(1, 2, 3, 5, 6);

    EXPECT_EQ(patternLine, equalPatternLine);
    EXPECT_NE(patternLine, notEqualPatternLine1);
    EXPECT_NE(patternLine, notEqualPatternLine2);
    EXPECT_NE(patternLine, notEqualPatternLine3);
    EXPECT_NE(patternLine, notEqualPatternLine4);
    EXPECT_NE(patternLine, notEqualPatternLine5);
}

TEST(SUITE_NAME, TestOutputOperator) {
    PatternLineN patternLine(1, 2, 3, 4, 5);
    std::stringstream ss;

    ss << patternLine;

    EXPECT_EQ("PatternLine{rgbn=RGBN{r=1, g=2, b=3, n=4}, fadeMillis=5}", ss.str());
}
