#include <blink1-lib.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <signal.h>

#include "config.hpp"
#include "blink-lib.hpp"

using namespace blink1_control::config;
using namespace blink1_control::blink1_lib;

static bool LOOPING = true;

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
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " config_file\n";
        return 1;
    }

    signal(SIGINT, signalCallbackHandler);

    ConfigParser parser;
    std::ifstream configFileStream(argv[1]);

    if (!configFileStream.is_open()) {
        std::cout << "Could not open " << argv[1] << "\n";
        return 2;
    }

    auto config = parser.parseConfig(configFileStream);

    Blink1Device blinkDevice;
    blinkDevice.setBlocking();

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
                    std::cout << "        Playing " << patternLine << "\n";
                    blinkDevice.fadeToRGBN(patternLine.fadeMillis, patternLine.rgbn);
                }
            }
        }
    }
}
