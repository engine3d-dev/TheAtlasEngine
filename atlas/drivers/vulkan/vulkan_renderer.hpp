#pragma once
#include <drivers/renderer_backend.hpp>
#include <string>
#include <vulkan/vulkan_core.h>


namespace atlas::vk {
    /**
     * @name vk_renderer
     * @note vulkan-backend specific renderer
     * @note renderer that is implemented with the vulkan's API
     * @note Provide an API on directly submitting tasks to the vulkan API that
     * gets send directly to the GPU
     */
    class vk_renderer : public render_context {
    public:
        vk_renderer(const std::string& Tag);

        //! @note Implementation-details for sending draw calls.
        VkCommandBuffer get_current_command_buffer();
        static VkCommandBuffer current_command_buffer();
        static VkFramebuffer current_frame_buffer();
        static uint32_t get_current_command_buffer_index();

    private:
        void begin_frame() override;
        void end_frame() override;
        // void set_initial_camera(PerspectiveCamera& camera) override;
        void submit_scene_object(ref<scene_object>& p_scene_object) override;
        void set_initial_camera(ref<scene_object>& p_scene_object) override;
        void draw_scene(ref<scene_object> p_scene_object) override;
        void draw_scene_objects(const ref<scene_scope>& p_scene) override;

        void draw_scene_object(ref<scene_object>& p_current_object) override;

        void draw_object_with_camera(ref<scene_object>& p_object,
                                     ref<scene_object>& p_camera) override;

    private:
        //! @note TODO: Moving VkPipeline/VkPipelineLayout out of the renderer
        //! and asbtracting this.
        void initialize_pipeline();

    private:
    };
};