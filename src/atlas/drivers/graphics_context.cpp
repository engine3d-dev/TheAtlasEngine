#include <drivers/graphics_context.hpp>
#include <core/application.hpp>
#include <drivers/vulkan/vulkan_context.hpp>

namespace atlas {

    void graphics_context::destroy() {
        return impl_destroy();
    }

    ref<graphics_context> initialize_context() {
        switch (application::current_api()){
        case atlas::API::VULKAN: return create_ref<vk::vk_context>();
        default: return nullptr;
        }

        return nullptr;
    }
};