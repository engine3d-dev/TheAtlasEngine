module;

#include <cstdint>

export module atlas.graphics_api;


export namespace atlas {
    enum class graphics_api : uint8_t {
        vulkan, dx11, fx12, undefined
    };
};