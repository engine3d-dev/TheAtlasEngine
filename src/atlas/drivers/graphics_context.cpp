#include <drivers/graphics_context.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

namespace atlas {
    ref<graphics_context> initialize_context(const std::string& p_tag) {
        switch (application::current_api()) {
            case api::vulkan:
                return create_ref<vk::vk_context>(p_tag);
            default:
                return nullptr;
        }
    }
};