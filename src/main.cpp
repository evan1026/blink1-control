#include <blink1-lib.h>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <span>

#include "blink-lib.hpp"
#include "config.hpp"

#ifdef USE_BLINK1_TESTING_LIBRARY
    #include "Blink1TestingLibrary.hpp"
#endif

using blink1_control::config::ConfigParser;
using blink1_control::config::PatternConfig;
using blink1_lib::Blink1Device;

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

std::optional<blink1_control::config::Config> parseArgs(int argc, const char* argv[]) {
    auto args = std::span(argv, static_cast<size_t>(argc));

    if (args.size() != 2) {
        std::cout << "Usage: " << args[0] << " config_file\n";
        return std::nullopt;
    }

    std::ifstream configFileStream(args[1]);

    if (!configFileStream.is_open()) {
        std::cout << "Could not open " << args[1] << "\n";
        return std::nullopt;
    }

    return ConfigParser::parseConfig(configFileStream);
}

void runPatterns(std::optional<blink1_control::config::Config> config) {

#ifdef USE_BLINK1_TESTING_LIBRARY
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(true);
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_INIT(true);
#endif

    Blink1Device blinkDevice;

    if (!blinkDevice.good()) {
        std::cout << "Could not open blink1 device\n";
        return;
    }

    blinkDevice.clearOnExit = true;

    while (LOOPING) {
        for (auto it = config->patternConfigs.begin(); it != config->patternConfigs.end() && LOOPING; ++it) {
            PatternConfig& pattern = *it->second;
            std::cout << "Playing " << pattern.name << "\n";

            std::cout << "    Playing before pattern\n";
            for (auto& patternLine : pattern.before) {
                std::cout << "        Playing " << *patternLine << "\n";
                patternLine->execute(blinkDevice);
            }

            for (int i = 0; i < pattern.repeat && LOOPING; ++i) {
                std::cout << "    Playing iteration " << i << "/" << pattern.repeat << "\n";
                for (auto& patternLine : pattern.pattern) {
                    std::cout << "        Playing " << *patternLine << "\n";
                    patternLine->execute(blinkDevice);
                }
            }

            std::cout << "    Playing after pattern\n";
            for (auto& patternLine : pattern.after) {
                std::cout << "        Playing " << *patternLine << "\n";
                patternLine->execute(blinkDevice);
            }
        }
    }
}

int main(int argc, const char* argv[]) {

    signal(SIGINT, signalCallbackHandler);

    auto config = parseArgs(argc, argv);

    if (!config) {
        return 1;
    }

    runPatterns(config);
}
