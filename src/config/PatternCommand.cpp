#include <thread>

#include "config/PatternCommand.hpp"


namespace blink1_control::config {
    FadeCommand::FadeCommand(blink1_lib::PatternLineN _fadeParams) : fadeParams(_fadeParams) {}

    void FadeCommand::execute(blink1_lib::Blink1Device& device) {
        device.fadeToRGBN(fadeParams.fadeMillis, fadeParams.rgbn);
    }

    WaitCommand::WaitCommand(std::chrono::milliseconds _waitTime) : waitTime(_waitTime) {}

    void WaitCommand::execute([[maybe_unused]] blink1_lib::Blink1Device& device) {
        std::this_thread::sleep_for(waitTime);
    }
}
