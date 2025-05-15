#pragma once
#include <drivers/graphics_context.hpp>
#include <drivers/vulkan/vulkan_driver.hpp>
#include <vulkan/vulkan.hpp>
#include <functional>
#include <queue>

namespace atlas::vk {
    class vk_context : public graphics_context {
    public:
        vk_context();
        ~vk_context() override;

        static VkInstance get_vk_instance() {
            return s_instance->m_vulkan_instance_context;
        }

        static vk_physical_driver& get_current_selected_physical_driver() {
            return s_instance->m_physical_driver;
        }

        static vk_driver& get_current_driver() { return s_instance->m_driver; }

        static void submit_resource_free(
          const std::function<void()>& p_callback_resource);

    private:
        std::vector<const char*> initialization_validation_layers();

        std::vector<const char*> initialize_instance_extensions();

    private:
        void impl_destroy() override;

    private:
        static vk_context* s_instance;
        VkInstance m_vulkan_instance_context=nullptr;
        vk_physical_driver m_physical_driver;
        vk_driver m_driver;
        std::deque<std::function<void()>> m_resource_free_queue_submission;
    };
};