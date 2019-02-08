#include <sstream>

#include "gtest/gtest.h"
#include "PatternLineN.hpp"

#define SUITE_NAME PatternLineN_test

using namespace blink1_control;

TEST(SUITE_NAME, TestConstructor) {
    PatternLineN patternLine1;
    PatternLineN patternLine2(RGBN(1, 2, 3, 4), 5);
    PatternLineN patternLine3(6, 7, 8, 9, 10);

    ASSERT_EQ(0, patternLine1.rgbn.r);
    ASSERT_EQ(0, patternLine1.rgbn.g);
    ASSERT_EQ(0, patternLine1.rgbn.b);
    ASSERT_EQ(0, patternLine1.rgbn.n);
    ASSERT_EQ(0, patternLine1.fadeMillis);

    ASSERT_EQ(1, patternLine2.rgbn.r);
    ASSERT_EQ(2, patternLine2.rgbn.g);
    ASSERT_EQ(3, patternLine2.rgbn.b);
    ASSERT_EQ(4, patternLine2.rgbn.n);
    ASSERT_EQ(5, patternLine2.fadeMillis);

    ASSERT_EQ(6, patternLine3.rgbn.r);
    ASSERT_EQ(7, patternLine3.rgbn.g);
    ASSERT_EQ(8, patternLine3.rgbn.b);
    ASSERT_EQ(9, patternLine3.rgbn.n);
    ASSERT_EQ(10, patternLine3.fadeMillis);
}

TEST(SUITE_NAME, TestEqual) {
    PatternLineN patternLine(1, 2, 3, 4, 5);
    PatternLineN equalPatternLine(1, 2, 3, 4, 5);
    PatternLineN notEqualPatternLine1(2, 2, 3, 4, 5);
    PatternLineN notEqualPatternLine2(1, 3, 3, 4, 5);
    PatternLineN notEqualPatternLine3(1, 2, 4, 4, 5);
    PatternLineN notEqualPatternLine4(1, 2, 3, 5, 5);
    PatternLineN notEqualPatternLine5(1, 2, 3, 5, 6);

    ASSERT_EQ(patternLine, equalPatternLine);
    ASSERT_NE(patternLine, notEqualPatternLine1);
    ASSERT_NE(patternLine, notEqualPatternLine2);
    ASSERT_NE(patternLine, notEqualPatternLine3);
    ASSERT_NE(patternLine, notEqualPatternLine4);
    ASSERT_NE(patternLine, notEqualPatternLine5);
}

TEST(SUITE_NAME, TestOutputOperator) {
    PatternLineN patternLine(1, 2, 3, 4, 5);
    std::stringstream ss;

    ss << patternLine;

    ASSERT_EQ("PatternLine{rgbn=RGBN{r=1, g=2, b=3, n=4}, fadeMillis=5}", ss.str());
}
