#pragma once
#include <drivers/graphics_context.hpp>
#include <vulkan/vulkan.h>
#include <deque>
#include <drivers/vulkan-cpp/vk_driver.hpp>

namespace atlas::vk {
    /**
     * @brief vulkan-specific implementation that is the backend of the graphics
     * contextd
     */
    class vk_context : public graphics_context {
    public:
        vk_context(const std::string& p_tag);

        /**
         * @brief used for providing a way to submit all vulkan metaobjects
         * before the destruction of the vulkan logical device
         *
         * Per vulkan specification, it is required to have all object handles
         * created with the logical device to be destroyed before the logical
         * device itself gets destroyed during post cleanup
         *
         * This function was a means to ensure that the destruction of those
         * vulkan child objects are handled in that order correctly
         */
        static void submit_resource_free(std::function<void()>&& p_resource);

        /**
         * @brief returns function pointer to allow for setting debug object name
         * 
         * 
         * This allows for utilizing vkSetDebugUtilsObjectNameEXT during debug builds
         * 
         * This allows for setting up object names that is useful to the programmer when a validation layer error message occurs unexpectedly
         * 
        */
        static PFN_vkSetDebugUtilsObjectNameEXT get_debug_object_name() {
            return s_instance->m_vk_set_debug_utils_object_name_ext;
        }

        /**
         * @brief Gives you direct access to the vulkan instance
         */
        static VkInstance handler();

        /**
         * @brief direct access to the vulkan physical device
         */
        static vk_physical_driver physical_driver() {
            return s_instance->m_physical;
        }

        /**
         * @brief direct access to the vulkan logical device
         */
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

        PFN_vkSetDebugUtilsObjectNameEXT m_vk_set_debug_utils_object_name_ext;
    };
};