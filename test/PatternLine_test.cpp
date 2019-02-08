#include <sstream>

#include "gtest/gtest.h"
#include "PatternLine.hpp"

#define SUITE_NAME PatternLine_test

using namespace blink1_control;

TEST(SUITE_NAME, TestConstructor) {
    PatternLine patternLine1;
    PatternLine patternLine2(RGB(1, 2, 3), 4);
    PatternLine patternLine3(5, 6, 7, 8);

    ASSERT_EQ(0, patternLine1.rgb.r);
    ASSERT_EQ(0, patternLine1.rgb.g);
    ASSERT_EQ(0, patternLine1.rgb.b);
    ASSERT_EQ(0, patternLine1.fadeMillis);

    ASSERT_EQ(1, patternLine2.rgb.r);
    ASSERT_EQ(2, patternLine2.rgb.g);
    ASSERT_EQ(3, patternLine2.rgb.b);
    ASSERT_EQ(4, patternLine2.fadeMillis);

    ASSERT_EQ(5, patternLine3.rgb.r);
    ASSERT_EQ(6, patternLine3.rgb.g);
    ASSERT_EQ(7, patternLine3.rgb.b);
    ASSERT_EQ(8, patternLine3.fadeMillis);
}

TEST(SUITE_NAME, TestEqual) {
    PatternLine patternLine(1, 2, 3, 4);
    PatternLine equalPatternLine(1, 2, 3, 4);
    PatternLine notEqualPatternLine1(2, 2, 3, 4);
    PatternLine notEqualPatternLine2(1, 3, 3, 4);
    PatternLine notEqualPatternLine3(1, 2, 4, 4);
    PatternLine notEqualPatternLine4(1, 2, 3, 5);

    ASSERT_EQ(patternLine, equalPatternLine);
    ASSERT_NE(patternLine, notEqualPatternLine1);
    ASSERT_NE(patternLine, notEqualPatternLine2);
    ASSERT_NE(patternLine, notEqualPatternLine3);
    ASSERT_NE(patternLine, notEqualPatternLine4);
}

TEST(SUITE_NAME, TestOutputOperator) {
    PatternLine patternLine(1, 2, 3, 4);
    std::stringstream ss;

    ss << patternLine;

    ASSERT_EQ("PatternLine{rgb=RGB{r=1, g=2, b=3}, fadeMillis=4}", ss.str());
}
