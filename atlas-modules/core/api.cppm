module;

#include <cstdint>

export module core:api;


export namespace atlas {
    enum class graphics_api : uint8_t {
        vulkan, dx11, fx12, undefined
    };
};