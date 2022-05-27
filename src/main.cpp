#include <blink1-lib.h>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <span>

#include "blink-lib.hpp"
#include "config.hpp"

#ifdef USE_FAKE_BLINK1TOOL
    #include "blink-lib/FakeBlink1Lib.hpp"
#endif

using blink1_control::config::ConfigParser;
using blink1_control::config::PatternConfig;
using blink1_control::blink1_lib::Blink1Device;

static bool LOOPING = true; // NOLINT

void signalCallbackHandler(int signum) {
    if (signum == SIGINT) {
        if (LOOPING) {
            std::cout << "\nCaught ctrl-c, will quit after the current pattern is done (hit ctrl-c again to force quit)\n";
            LOOPING = false;
        } else {
            std::cout << "\nFORCE QUIT\n";
            exit(4);
        }
    }
}

int main(int argc, const char* argv[]) {
    auto args = std::span(argv, size_t(argc));

    if (args.size() != 2) {
        std::cout << "Usage: " << args[0] << " config_file\n";
        return 1;
    }

    signal(SIGINT, signalCallbackHandler);

    std::ifstream configFileStream(args[1]);

    if (!configFileStream.is_open()) {
        std::cout << "Could not open " << args[1] << "\n";
        return 2;
    }

    auto config = ConfigParser::parseConfig(configFileStream);


#ifdef USE_FAKE_BLINK1TOOL
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(true);
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_INIT(true);
#endif

    Blink1Device blinkDevice;

    if (!blinkDevice.good()) {
        std::cout << "Could not open blink1 device\n";
        return 3;
    }

    while (LOOPING) {
        for (auto it = config->patternConfigs.begin(); it != config->patternConfigs.end() && LOOPING; ++it) {
            PatternConfig& pattern = *it->second;
            std::cout << "Playing " << pattern.name << "\n";

            for (int i = 0; i < pattern.repeat && LOOPING; ++i) {
                std::cout << "    Playing iteration " << i << "/" << pattern.repeat << "\n";
                for (auto& patternLine : pattern.pattern) {
                    std::cout << "        Playing " << *patternLine << "\n";
                    patternLine->execute(blinkDevice);
                }
            }
        }
    }
}
