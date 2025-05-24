#pragma once
#include <drivers/graphics_context.hpp>
#include <drivers/vulkan/vulkan_driver.hpp>
#include <vulkan/vulkan.hpp>
#include <deque>

namespace atlas::vk {
    class vk_context : public graphics_context{
    public:
        vk_context(const std::string& p_tag);

        ~vk_context() override;

        static VkInstance vk_instance_handler() { return s_instance->m_instance; }

        static vk_physical_driver& current_physical_driver() {
            return s_instance->m_physical_driver;
        }

        static vk_driver& get_current_driver() { return s_instance->m_driver; }

        operator VkInstance() { return m_instance; }

        static void submit_context_free(std::function<void()>&& p_task);

    private:
        void free_resource(std::function<void()>& p_task);
        void destroy_context() override;

    private:
        // static std::vector<const char*> initialization_validation_layers();

        // static std::vector<const char*> initialize_instance_extensions();

    private:
        static vk_context* s_instance;
        VkInstance m_instance=nullptr;
        vk_physical_driver m_physical_driver;
        vk_driver m_driver;

        std::deque<std::function<void()>> m_resources_freed{};
    };
};