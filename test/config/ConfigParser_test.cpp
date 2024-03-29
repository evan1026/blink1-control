#include <sstream>
#include <fstream>

#include "gtest/gtest.h"
#include "config/ConfigParser.hpp"
#include "config/ProcessMonitorConfig.hpp"
#include "config/RollupConfig.hpp"

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
        "socketPath": "path.sock",
        "conditions": [
            {
                "name": "pm_test",
                "type": "ProcessMonitor",
                "patterns": [],
                "cmd": "test cmd 1"
            },
            {
                "name": "pm_test2",
                "type": "ProcessMonitor",
                "patterns": [],
                "cmd": "test cmd 2"
            },
            {
                "name": "rollup_test",
                "type": "Rollup",
                "patterns": [
                    "rollup_pattern"
                ],
                "children": [
                    {
                        "name": "pm_test",
                        "critical": true
                    },
                    {
                        "name": "pm_test2",
                        "critical": false
                    }
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
                    },
                    {
                        "time": 1234
                    }
                ],
                "before": [
                    {
                        "color": "#ABCDEF",
                        "led": 5,
                        "time": 10
                    }
                ],
                "after": [
                    {
                        "color": "#FEDCBA",
                        "led": 6,
                        "time": 11
                    }
                ]
            }
        ]
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    ASSERT_TRUE(bool(configOpt));

    auto config = *configOpt;
    EXPECT_EQ("path.sock", config.socketPath);
    EXPECT_EQ(3, config.conditionConfigs.size());
    EXPECT_EQ(1, config.patternConfigs.size());

    ASSERT_TRUE(config.conditionConfigs.find("pm_test") != config.conditionConfigs.end());
    ASSERT_TRUE(config.conditionConfigs.find("pm_test2") != config.conditionConfigs.end());
    ASSERT_TRUE(config.conditionConfigs.find("rollup_test") != config.conditionConfigs.end());
    ASSERT_TRUE(config.patternConfigs.find("rollup_pattern") != config.patternConfigs.end());

    auto pmTestConfig = config.conditionConfigs.at("pm_test");
    auto pmTest2Config = config.conditionConfigs.at("pm_test2");
    auto rollupTestConfig = config.conditionConfigs.at("rollup_test");
    auto rollupPatternConfig = config.patternConfigs.at("rollup_pattern");

    EXPECT_EQ("pm_test", pmTestConfig->name);
    EXPECT_EQ(ConditionConfig::Type::ProcessMonitor, pmTestConfig->type);
    EXPECT_EQ(0, pmTestConfig->patterns.size());
    EXPECT_EQ("test cmd 1", dynamic_cast<ProcessMonitorConfig&>(*pmTestConfig).cmd);

    EXPECT_EQ("pm_test2", pmTest2Config->name);
    EXPECT_EQ(ConditionConfig::Type::ProcessMonitor, pmTest2Config->type);
    EXPECT_EQ(0, pmTest2Config->patterns.size());
    EXPECT_EQ("test cmd 2", dynamic_cast<ProcessMonitorConfig&>(*pmTest2Config).cmd);

    EXPECT_EQ("rollup_test", rollupTestConfig->name);
    EXPECT_EQ(ConditionConfig::Type::Rollup, rollupTestConfig->type);
    EXPECT_EQ(1, rollupTestConfig->patterns.size());
    EXPECT_EQ("rollup_pattern", rollupTestConfig->patterns[0]);

    auto& rollupChildren = dynamic_cast<RollupConfig&>(*rollupTestConfig).children;
    ASSERT_EQ(2, rollupChildren.size());
    EXPECT_EQ("pm_test", rollupChildren[0].name);
    EXPECT_TRUE(rollupChildren[0].critical);
    EXPECT_EQ("pm_test2", rollupChildren[1].name);
    EXPECT_FALSE(rollupChildren[1].critical);

    EXPECT_EQ("rollup_pattern", rollupPatternConfig->name);
    EXPECT_EQ(7, rollupPatternConfig->repeat);
    EXPECT_EQ(3, rollupPatternConfig->pattern.size());

    auto& patternLine1 = *rollupPatternConfig->pattern[0];
    auto& patternLine2 = *rollupPatternConfig->pattern[1];
    auto& patternLine3 = *rollupPatternConfig->pattern[2];

    EXPECT_EQ(typeid(FadeCommand&), typeid(patternLine1));
    auto& fadeCommand1 = dynamic_cast<FadeCommand&>(patternLine1);
    EXPECT_EQ(255, fadeCommand1.fadeParams.rgbn.r);
    EXPECT_EQ(254, fadeCommand1.fadeParams.rgbn.g);
    EXPECT_EQ(253, fadeCommand1.fadeParams.rgbn.b);
    EXPECT_EQ(1, fadeCommand1.fadeParams.rgbn.n);
    EXPECT_EQ(50, fadeCommand1.fadeParams.fadeMillis);

    EXPECT_EQ(typeid(FadeCommand&), typeid(patternLine2));
    auto& fadeCommand2 = dynamic_cast<FadeCommand&>(patternLine2);
    EXPECT_EQ(0, fadeCommand2.fadeParams.rgbn.r);
    EXPECT_EQ(0, fadeCommand2.fadeParams.rgbn.g);
    EXPECT_EQ(0, fadeCommand2.fadeParams.rgbn.b);
    EXPECT_EQ(2, fadeCommand2.fadeParams.rgbn.n);
    EXPECT_EQ(100, fadeCommand2.fadeParams.fadeMillis);

    EXPECT_EQ(typeid(WaitCommand&), typeid(patternLine3));
    auto& waitCommand = dynamic_cast<WaitCommand&>(patternLine3);
    EXPECT_EQ(std::chrono::milliseconds(1234), waitCommand.waitTime);

    auto& beforeLine = *rollupPatternConfig->before[0];
    auto& afterLine = *rollupPatternConfig->after[0];

    EXPECT_EQ(typeid(FadeCommand&), typeid(beforeLine));
    auto& beforeCommand = dynamic_cast<FadeCommand&>(beforeLine);
    EXPECT_EQ(0xAB, beforeCommand.fadeParams.rgbn.r);
    EXPECT_EQ(0xCD, beforeCommand.fadeParams.rgbn.g);
    EXPECT_EQ(0xEF, beforeCommand.fadeParams.rgbn.b);
    EXPECT_EQ(5, beforeCommand.fadeParams.rgbn.n);
    EXPECT_EQ(10, beforeCommand.fadeParams.fadeMillis);

    EXPECT_EQ(typeid(FadeCommand&), typeid(afterLine));
    auto& afterCommand = dynamic_cast<FadeCommand&>(afterLine);
    EXPECT_EQ(0xFE, afterCommand.fadeParams.rgbn.r);
    EXPECT_EQ(0xDC, afterCommand.fadeParams.rgbn.g);
    EXPECT_EQ(0xBA, afterCommand.fadeParams.rgbn.b);
    EXPECT_EQ(6, afterCommand.fadeParams.rgbn.n);
    EXPECT_EQ(11, afterCommand.fadeParams.fadeMillis);
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

TEST_F(SUITE_NAME, TestInvalidCommand) {
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

TEST_F(SUITE_NAME, TestDefaultValues) {
    std::stringstream ss;
    ss << R"(
    {
        "conditions": [],
        "patterns": []
    })";

    ConfigParser configParser;
    auto configOpt = configParser.parseConfig(ss);
    EXPECT_TRUE(configOpt);

    EXPECT_EQ("./blink1-control.sock", configOpt->socketPath);
}
