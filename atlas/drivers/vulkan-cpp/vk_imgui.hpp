#pragma once
#include <string>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <GLFW/glfw3.h>
#include <drivers/vulkan-cpp/vk_driver.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <core/window.hpp>

namespace atlas::vk {
    struct hud_data {
        float playerHealth = 100.0f;
        int playerScore = 0;
        float fps = 0.0f;
        std::string currentWeapon = "Assault Rifle";
        // Add more data as needed
    };

    class imgui_context {
    public:
        imgui_context() = default;
        // imgui_context(GLFWwindow* p_window_handler,
        //               const vk_swapchain& p_current_swapchain_handler,
        //               const VkRenderPass& p_current_renderpass);
        imgui_context(const ref<window>& p_window_ctx);

        void recreate(GLFWwindow* p_window_handler,
                      const uint32_t& p_image_size,
                      const VkRenderPass& p_current_renderpass);

        void begin(const VkCommandBuffer& p_current,
                   const uint32_t& p_current_frame_idx);
        void end();

        // HUD Example, using this to test if imgui initialization works
        // note: experiemental, for testing imgui with new vulkan implementation
        // Proper API I want to have this be in atlas::ui::hud namespace
        // API Usage: ui::hud::health_bar(float)
        // If users have custom components they want to register to the UI, then
        // do:
        /*
            template<typename UComponent, typename UType>
            void custom_hud_component(UComponent* p_component,, UType p_type) {
                // then prob do something lik this: use template to associate
           struct component and the data, with type specified
                imgui::progress_bar((p_component)->*p_data);
            }

            User-side would be the following:
            struct custom_component_heatlh {
                uint32_t health_data = 10;
            };
            some_entity->set<custom_component_heatlh>({});
            const custom_component_health* player_hp =
           some_entity.get<custom_component_heatlh>();

            // Through this specifications the UI will now apply this to the hud
            // Questions that needs to be talked here are the following points:
            1.) How will users specify UI component uses?
                * Position of HUD
                * Handling alignment of the HUD
            2.) Ease of usability with minimal specifications on user-side for
           getting HUD's to work 3.) Making a class be optional, meaning have
           this be through imgui's API's as a function 4.) Asking if its needed
           to be represented as a class atlas::ui::hud::health_bar(player_hp,
           &custom_component_health::health_data)
        */
        void draw_hud(const hud_data& p_test,
                      const window_settings& p_settings);

    private:
        VkInstance m_instance = nullptr;
        VkPhysicalDevice m_physical = nullptr;
        vk_driver m_driver{};
        VkSwapchainKHR m_current_swapchain_handler = nullptr;

        VkDescriptorPool m_desc_pool = nullptr;
        VkCommandBuffer m_current_command = nullptr;
    };
};