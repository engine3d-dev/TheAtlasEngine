#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace atlas::vk {

    struct surface_properties {
        VkSurfaceCapabilitiesKHR surface_capabilities;
        VkSurfaceFormatKHR surface_format;
    };

    class vk_physical_driver {
        struct queue_family_indices {
            uint32_t graphics = -1;
            uint32_t compute = -1;
            uint32_t transfer = -1;
            uint32_t present = -1;
        };

    public:
        vk_physical_driver() = default;
        vk_physical_driver(const VkInstance& p_instance);
        ~vk_physical_driver();

        [[nodiscard]] queue_family_indices read_queue_family_indices() const {
            return m_queue_indices;
        }

        [[nodiscard]] uint32_t read_presentation_index(
          const VkSurfaceKHR& p_surface);

        [[nodiscard]] surface_properties get_surface_properties(
          const VkSurfaceKHR& p_surface);

        operator VkPhysicalDevice() { return m_physical_driver; }

        operator VkPhysicalDevice() const { return m_physical_driver; }

    private:
        queue_family_indices select_queue_family_indices();

    private:
        static vk_physical_driver* s_instance;
        VkPhysicalDevice m_physical_driver = nullptr;
        queue_family_indices m_queue_indices{};
        std::vector<VkQueueFamilyProperties> m_queue_family_properties{};
        surface_properties m_surface_properties{};
    };
};