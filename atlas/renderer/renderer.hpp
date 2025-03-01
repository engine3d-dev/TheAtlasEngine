#pragma once
#include <vulkan/vulkan.hpp>
#include <core/core.hpp>
#include <core/scene/scene.hpp>

namespace atlas{
    /**
     * @name Renderer
     * @note Is our actual public API for handling a lot of our renderer.
     * @note The way we have this setup is the renderer will have a renderer context.
     * @note Renderer context and what that means is defined by its implementation.
     * @note Meaning if we eventually handle multiple graphics API's that may be performant on specific platforms like DirectX.
     *      The renderer context's public API's wont change but the operations behind it's implementation would be different by the API being used and set
     * @note TODO: Some thing I will consider because currently at runtime these graphic API's can be set, this is something that should be set at compile-time
    */
    class scene_object;
    class renderer{
    public:
        //! @note Used to initialize
        static void initialize();

        static void set_camera(ref<scene_object>& p_object);

        static void submit_scene_object(ref<scene_object>& p_object);

        static uint32_t get_current_frame();

        // static void SetCustomShaders(const std::string& p_VertexShader, const std::string& p_FragmentShader, bool p_IsOveridden);
        // static bool IsCustomShaderEnabled(){
        //     return false;
        // }

        /**
        @note Begin()
            - Indicates to the renderer the start of the frame
        
        @note End()
            - Tells renderer when a frame ends

        TODO: Implement Batch Rendering (Minimal Optimizaiton)
            - Batch Rendering is a technique on rendering in batches
            - Meaning when we call `Render*` functions, we are not calling draw commands per function call of `Render*`
            - Batch Rendering is how we render in batches. Meaning when we call Render*, we submit to via those Render* functions. Then when we reached the end of the frame via End() function.
            - Then End() is reached before we indicate the scene is rendered, we render all our modified objects
        */
        static void begin();
        static void end();

        // static void set_initial_camera(PerspectiveCamera& camera);

        static void render_scene_objects(const ref<scene_scope>& p_scene);
        static void render_scene_object(ref<scene_object>& p_scene_object);

        static void render_with_camera(ref<scene_object>& p_scene_object, ref<scene_object>& p_camera_object);
    };
};