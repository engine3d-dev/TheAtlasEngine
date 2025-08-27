#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <string>
#include <filesystem>

namespace atlas {
    class serializer {
    public:
        serializer() = default;
        serializer(const ref<scene_scope>& p_scene_ctx);

        // void save_as(const std::string& p_filepath);
        void save(const std::filesystem::path& p_filepath);

        bool load(const std::filesystem::path& p_filepath,
                  const flecs::world& p_registry);

    private:
        ref<scene_scope> m_current_scene_ctx;
    };

}; // namespace atlas