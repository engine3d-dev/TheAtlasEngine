#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <string>

namespace atlas {
    class serializer {
    public:
        serializer(const scene_scope* p_scene_ctx);

        void save_as(const std::string& p_filepath);

    private:
        ref<scene_scope> m_current_scene_ctx;
    };

}; // namespace atlas