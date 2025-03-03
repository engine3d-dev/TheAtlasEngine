#pragma once
#include <drivers/vulkan/vulkan_driver.hpp>
#include <vulkan/vulkan.hpp>

namespace atlas::vk {
    class vk_context {
    public:
        static void initialize();
        static VkInstance get_vk_instance() { return s_instance; }

        static vk_physical_driver& get_current_selected_physical_driver() {
            return s_physical_driver;
        }

        static vk_driver& get_current_driver() { return s_driver; }

    private:
        static std::vector<const char*> initialization_validation_layers();

        static std::vector<const char*> initialize_instance_extensions();

    private:
        static VkInstance s_instance;
        static vk_physical_driver s_physical_driver;
        static vk_driver s_driver;
    };
};