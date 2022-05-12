
#include "gtest/gtest.h"
#include "config/PatternCommand.hpp"
#include "../blink-lib/FakeBlink1Lib.hpp"

using namespace blink1_control::config;
using namespace blink1_control::blink1_lib;

#define FADE_SUITE_NAME FadeCommand_test
#define WAIT_SUITE_NAME WaitCommand_test

class FADE_SUITE_NAME : public ::testing::Test {
    protected:
        void SetUp() override {
            fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(true);
            fake_blink1_lib::SET_BLINK1_SUCCESSFUL_INIT(true);
        }

        void TearDown() override {
            fake_blink1_lib::CLEAR_ALL();
        }
};

TEST_F(FADE_SUITE_NAME, TestExecute) {
    Blink1Device device;
    FadeCommand fadeCommand(PatternLineN(1, 2, 3, 4, 5));
    fadeCommand.execute(device);

    RGB actualRgb = fake_blink1_lib::GET_RGB(4);
    RGB expectRgb(1, 2, 3);
    std::uint16_t actualFadeMillis = fake_blink1_lib::GET_FADE_MILLIS(4);

    EXPECT_EQ(5, actualFadeMillis);
    EXPECT_EQ(expectRgb, actualRgb);
}

TEST_F(FADE_SUITE_NAME, TestOutputOperator) {
    FadeCommand fadeCommand(PatternLineN(1, 2, 3, 4, 5));
    std::stringstream ss;

    ss << fadeCommand;

    EXPECT_EQ("FadeCommand{fadeParams: PatternLine{rgbn=RGBN{r=1, g=2, b=3, n=4}, fadeMillis=5}}", ss.str());
}

TEST(WAIT_SUITE_NAME, TestExecute) {
    Blink1Device device;
    WaitCommand waitCommand(std::chrono::milliseconds(100));

    auto currentTime = std::chrono::system_clock::now();
    waitCommand.execute(device);
    auto timeAfterWait = std::chrono::system_clock::now();
    EXPECT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterWait - currentTime).count(), 100) << "Expected waitCommand.execute() to take at least 100ms";
}

TEST(WAIT_SUITE_NAME, TestOutputOperator) {
    WaitCommand waitCommand(std::chrono::milliseconds(100));
    std::stringstream ss;

    ss << waitCommand;

    EXPECT_EQ("WaitCommand{waitTime: 100ms}", ss.str());
}
