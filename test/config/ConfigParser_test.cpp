#include <sstream>
#include <fstream>

#include "gtest/gtest.h"
#include "config/ConfigParser.hpp"

using namespace blink1_control::config;

#define SUITE_NAME ConfigParser_test

class SUITE_NAME : public ::testing::Test {

    std::stringstream buffer;
    std::streambuf* sbuf;

    virtual void SetUp() {
        // Save cout's buffer here
        sbuf = std::cout.rdbuf();

        // Redirect cout to our stringstream buffer or any other ostream
        std::cout.rdbuf(buffer.rdbuf());
    }

    virtual void TearDown() {
        // When done redirect cout to its old self
        std::cout.rdbuf(sbuf);
    }
};

TEST_F(SUITE_NAME, TestParseConfig) {
    std::stringstream ss;
    ss << R"(
    {
        "conditions": [
            {
                "name": "pm_test",
                "type": "ProcessMonitor",
                "patterns": []
            },
            {
                "name": "rollup_test",
                "type": "Rollup",
                "patterns": [
                    "rollup_pattern"
                ]
            }
        ],

        "patterns": [
            {
                "name": "rollup_pattern",
                "repeat": 7,
                "lines": [
                    {
                        "color": "#FFFEFD",
                        "led": 1,
                        "time": 50
                    },
                    {
                        "color": "#000000",
                        "led": 2,
                        "time": 100
                    }
                ]
            }
        ]
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_TRUE(bool(configOpt));

    auto config = *configOpt;
    EXPECT_EQ(2, config.conditionConfigs.size());
    EXPECT_EQ(1, config.patternConfigs.size());

    EXPECT_TRUE(config.conditionConfigs.find("pm_test") != config.conditionConfigs.end());
    EXPECT_TRUE(config.conditionConfigs.find("rollup_test") != config.conditionConfigs.end());
    EXPECT_TRUE(config.patternConfigs.find("rollup_pattern") != config.patternConfigs.end());

    auto pmTestConfig = config.conditionConfigs["pm_test"];
    auto rollupTestConfig = config.conditionConfigs["rollup_test"];
    auto rollupPatternConfig = config.patternConfigs["rollup_pattern"];

    EXPECT_EQ("pm_test", pmTestConfig.name);
    EXPECT_EQ(ConditionConfig::Type::ProcessMonitor, pmTestConfig.type);
    EXPECT_EQ(0, pmTestConfig.patterns.size());

    EXPECT_EQ("rollup_test", rollupTestConfig.name);
    EXPECT_EQ(ConditionConfig::Type::Rollup, rollupTestConfig.type);
    EXPECT_EQ(1, rollupTestConfig.patterns.size());
    EXPECT_EQ("rollup_pattern", rollupTestConfig.patterns[0]);

    EXPECT_EQ("rollup_pattern", rollupPatternConfig.name);
    EXPECT_EQ(7, rollupPatternConfig.repeat);
    EXPECT_EQ(2, rollupPatternConfig.pattern.size());

    auto patternLine1 = rollupPatternConfig.pattern[0];
    auto patternLine2 = rollupPatternConfig.pattern[1];

    EXPECT_EQ(255, patternLine1.rgbn.r);
    EXPECT_EQ(254, patternLine1.rgbn.g);
    EXPECT_EQ(253, patternLine1.rgbn.b);
    EXPECT_EQ(1, patternLine1.rgbn.n);
    EXPECT_EQ(50, patternLine1.fadeMillis);

    EXPECT_EQ(0, patternLine2.rgbn.r);
    EXPECT_EQ(0, patternLine2.rgbn.g);
    EXPECT_EQ(0, patternLine2.rgbn.b);
    EXPECT_EQ(2, patternLine2.rgbn.n);
    EXPECT_EQ(100, patternLine2.fadeMillis);
}

TEST_F(SUITE_NAME, TestBadStreams) {
    std::stringstream ss;

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_FALSE(bool(configOpt));

    std::ifstream fakeFileInput("/fakeFileThatShouldNotExistGodHelpYouIfItDoes");
    configOpt = configParser.parseConfig(fakeFileInput);
    EXPECT_FALSE(bool(configOpt));
}

TEST_F(SUITE_NAME, TestBadConditionPattern) {
    std::stringstream ss;
    ss << R"(
    {
        "conditions": [
            {
                "name": "pm_test",
                "type": "ProcessMonitor",
                "patterns": []
            },
            {
                "name": "rollup_test",
                "type": "Rollup",
                "patterns": [
                    {"asd":"asd"}
                ]
            }
        ]
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_FALSE(bool(configOpt));
}

TEST_F(SUITE_NAME, TestBadCondition) {
    std::stringstream ss;
    ss << R"(
    {
        "conditions": [
            {
            }
        ]
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_FALSE(bool(configOpt));
}

TEST_F(SUITE_NAME, TestBadPattern) {
    std::stringstream ss;
    ss << R"(
    {
        "patterns": [
            {
            }
        ]
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_FALSE(bool(configOpt));
}

TEST_F(SUITE_NAME, TestBadRgb) {
    std::stringstream ss;
    ss << R"(
    {
        "conditions": [
        ],

        "patterns": [
            {
                "name": "rollup_pattern",
                "repeat": 7,
                "lines": [
                    {
                        "color": "asdasd",
                        "led": 1,
                        "time": 50
                    }
                ]
            }
        ]
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_FALSE(bool(configOpt));
}
