#include <blink1-lib.h>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <span>
#include <thread>
#include <vector>

#include "blink-lib.hpp"
#include "config.hpp"
#include "Socket.hpp"

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


#ifdef USE_BLINK1_TESTING_LIBRARY
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_OPERATION(true);
    fake_blink1_lib::SET_BLINK1_SUCCESSFUL_INIT(true);
#endif

    blink1_control::Socket socket(blink1_control::SocketDomain::Local, blink1_control::SocketType::Stream, blink1_control::SocketProtocol::IP);
    if (socket.getStatus() == blink1_control::SocketStatus::NotOpen) {
        std::cout << "Error: " << socket.getError() << "\n";
        return 4;
    }
    std::cout << "Socket status: " << socket.getStatus() << ", Socket File Descriptor: " << socket.getSocketFd() << "\n";

    auto bindStatus = socket.bind("./blink1-control.sock");
    std::cout << "Bind status: " << bindStatus << ", Socket status: " << socket.getStatus() << ", Socket Bind Path: " << socket.getPath() << "\n";
    if (bindStatus != blink1_control::SocketError::None) {
        return 5;
    }

    auto listenStatus = socket.listen();
    std::cout << "Listen status: " << listenStatus << ", Socket status: " << socket.getStatus() << "\n";
    if (listenStatus != blink1_control::SocketError::None) {
        return 6;
    }

    std::cout << "Nonblocking status: " << socket.setNonBlocking() << "\n";

    std::vector<blink1_control::SocketConnection> socketConnections;

    while (LOOPING) {
        auto returnVal = socket.accept();

        if (returnVal.first != blink1_control::SocketError::WouldBlockTryAgain) {
            std::cout << "Accept status: " << returnVal.first << "\n";

            if (returnVal.first == blink1_control::SocketError::None) {
                std::cout << "New Connection Address: " << returnVal.second->getAddress() << "\n";
                socketConnections.push_back(std::move(*returnVal.second));
            }
        }

        for (auto it = socketConnections.begin(); it != socketConnections.end();) {
            auto& connection = *it;
            auto receivedData = connection.receive();

            if (receivedData.first == blink1_control::SocketError::None) {
                std::cout << "Received data: " << receivedData.second << "\n";
                ++it;
            } else if (receivedData.first == blink1_control::SocketError::SocketClosed) {
                std::cout << "Connection Closed\n";
                it = socketConnections.erase(it);
            } else {
                std::cout << "Error receiving: " << receivedData.first << "\n";
                ++it;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Blink1Device blinkDevice;

    if (!blinkDevice.good()) {
        std::cout << "Could not open blink1 device\n";
        return 3;
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
