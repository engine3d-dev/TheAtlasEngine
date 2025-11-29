#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <string>
#include <filesystem>

namespace atlas {
    /**
     * @brief serializer is responsible for saving/loading scenes
     *
     * This class excepts a scene context with the purpose of serializing with
     * the entities that reside within that specific scene context
     *
     * Which also provide an API for loading in a scene from disk for
     * deserialization.
     */
    class serializer {
    public:
        serializer() = default;

        /**
         * @brief constructs a new serializer with a requirement to specify a
         * scene to serialize
         * @param p_scene_ctx is the current scene to perform
         * serialization/deserialization to
         */
        serializer(const ref<scene>& p_scene_ctx);

        /**
         * @param p_filepath is the specified path to save the file
         */
        void save(const std::filesystem::path& p_filepath);

        /**
         * @param p_filepath is the specified path to loading in the saved file
         * from
         * @param p_registry is the current scene registry to load/create the
         * entities through that registry
         * @return true if loading was successful, otherwise will return false
         */
        bool load(const std::filesystem::path& p_filepath,
                  const flecs::world& p_registry);

    private:
        ref<scene> m_current_scene_ctx;
    };

}; // namespace atlas