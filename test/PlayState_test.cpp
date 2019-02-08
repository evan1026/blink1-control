
#include <sstream>

#include "gtest/gtest.h"
#include "PlayState.hpp"

#define SUITE_NAME PlayState_test

using namespace blink1_control;

TEST(SUITE_NAME, TestConstructor) {
    PlayState playState1;
    PlayState playState2(true, 1, 2, 3, 4);

    ASSERT_EQ(false, playState1.playing);
    ASSERT_EQ(0, playState1.playStart);
    ASSERT_EQ(0, playState1.playEnd);
    ASSERT_EQ(0, playState1.playCount);
    ASSERT_EQ(0, playState1.playPos);

    ASSERT_EQ(true, playState2.playing);
    ASSERT_EQ(1, playState2.playStart);
    ASSERT_EQ(2, playState2.playEnd);
    ASSERT_EQ(3, playState2.playCount);
    ASSERT_EQ(4, playState2.playPos);
}

TEST(SUITE_NAME, TestEqual) {
    PlayState playState(false, 1, 2, 3, 4);
    PlayState equalPlayState(false, 1, 2, 3, 4);
    PlayState notEqualPlayState1(true,  1, 2, 3, 4);
    PlayState notEqualPlayState2(false, 2, 2, 3, 4);
    PlayState notEqualPlayState3(false, 1, 3, 3, 4);
    PlayState notEqualPlayState4(false, 1, 2, 4, 4);
    PlayState notEqualPlayState5(false, 1, 2, 3, 5);

    ASSERT_EQ(playState, equalPlayState);
    ASSERT_NE(playState, notEqualPlayState1);
    ASSERT_NE(playState, notEqualPlayState2);
    ASSERT_NE(playState, notEqualPlayState3);
    ASSERT_NE(playState, notEqualPlayState4);
    ASSERT_NE(playState, notEqualPlayState5);
}

TEST(SUITE_NAME, TestOutputOperator) {
    PlayState playState(true, 1, 2, 3, 4);
    std::stringstream ss;

    ss << playState;
    ASSERT_EQ("PlayState{playing=true, playStart=1, playEnd=2, playCount=3, playPos=4}", ss.str());

    ss.str("");
    playState.playing = false;
    ss << playState;
    ASSERT_EQ("PlayState{playing=false, playStart=1, playEnd=2, playCount=3, playPos=4}", ss.str());
}
