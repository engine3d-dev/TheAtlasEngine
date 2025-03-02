#pragma once
#include <core/core.hpp>
#include <core/scene/components.hpp>
#include <core/scene/scene.hpp>
#include <string>

namespace atlas {
    class scene_object;
    class render_context {
    public:
        static scope<render_context> initialize(const std::string& p_tag);
        virtual ~render_context() = default;

        void begin();
        void end();

        static uint32_t get_current_frame();

        void set_camera_object(ref<scene_object>& camera) {
            return set_initial_camera(camera);
        }
        void submit_render_scene_object(ref<scene_object>& p_Object) {
            return submit_scene_object(p_Object);
        }

        void render_scene_object(ref<scene_object>& p_scene_object);
        void render_scene_objects(const ref<scene_scope>& p_scene_ctx);
        void render_with_camera(ref<scene_object>& p_scene_object,
                                ref<scene_object>& p_camera);

    private:
        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;

        virtual void submit_scene_object(ref<scene_object>& p_scene_object) = 0;
        virtual void set_initial_camera(ref<scene_object>& p_camera) = 0;

        virtual void draw_scene(ref<scene_object> p_scene_object) = 0;
        virtual void draw_scene_objects(
          const ref<scene_scope>& p_scene_ctx) = 0;
        virtual void draw_scene_object(ref<scene_object>& p_scene_object) = 0;
        virtual void draw_object_with_camera(ref<scene_object>& p_scene_object,
                                             ref<scene_object>& p_camera) = 0;
    };
}; // namespace atlas