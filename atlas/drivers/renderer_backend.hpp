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
    private:
        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;
    };
}; // namespace atlas