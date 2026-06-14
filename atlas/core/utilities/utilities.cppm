module;

#include <cstdint>
#include <string>

export module atlas.core.utilities;

export import :logger;
export import :common;
export import :poll_state;
export import :state;

namespace atlas {
    export struct window_params {
        uint32_t width;
        uint32_t height;
        std::string name = "";
    };
};