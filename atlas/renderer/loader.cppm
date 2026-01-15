module;

#include <string>
#include <cstdint>

export module atlas.renderer.loader;

import atlas.core.utilities;

import atlas.drivers.graphics_context;

import atlas.drivers.vulkan.instance_context;


export namespace atlas {
    /**
     * @brief construct a new graphics context and initializes that API
     * @return shared_ptr<graphics_context>
     */
    ref<graphics_context> initialize_context(const std::string& p_name, graphics_api p_api) {
        switch(p_api) {
            case graphics_api::vulkan:
                return create_ref<vulkan::instance_context>(p_name);
            default:
                return nullptr;
        }
    }
};