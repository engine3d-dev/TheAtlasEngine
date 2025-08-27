#pragma once
#include <drivers/graphics_context.hpp>
#include <vulkan/vulkan.h>
#include <deque>
#include <drivers/vulkan-cpp/vk_driver.hpp>

namespace atlas::vk {
    class vk_context : public graphics_context {
    public:
        vk_context(const std::string& p_tag);

        static void submit_resource_free(std::function<void()>&& p_resource);

        static VkInstance handler();

        static vk_physical_driver physical_driver() {
            return s_instance->m_physical;
        }

        static vk_driver driver_context() { return s_instance->m_driver; }

    private:
        void resource_free(std::function<void()>&& p_resource);

    private:
        void destroy_context() override;

    private:
        static vk_context* s_instance;
        VkInstance m_instance_handler = nullptr;
        vk_physical_driver m_physical{};
        vk_driver m_driver{};
        std::deque<std::function<void()>> m_resources_free{};
    };
};