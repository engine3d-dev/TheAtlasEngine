#pragma once
#include <cstdint>
#include <string>

namespace atlas {
    /**
     * @brief Specific settings to the window configuration
     * @brief Contains window properties for doing things with the windows
    */
    struct window_settings {
        uint32_t width = -1;
        uint32_t height = -1;
        std::string name = "";
        uint32_t frames_in_flight = 2;
    };
};