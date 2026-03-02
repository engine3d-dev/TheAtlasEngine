module;

#include <string>
#include <flecs.h>
#include <map>
#include <stdexcept>

export module atlas.core.scene.world;

import atlas.common;
import atlas.core.scene;
import atlas.core.level_streamer;
import atlas.core.event;

export namespace atlas {
    /**
     * @brief world represents a larger scope of areas that manages the scene
     * contexts
     *
     * The world is responsible for providing a self-containment of scenes and
     * managing which scenes are current. Based on current scenes are details
     * the world can know about.
     *
     * Details such as through player transform or navigation that can be done
     * through the world, providing useful information on where a game object in
     * those scenes/levels.
     *
     * The world responsibility act as the based container of any scene that can
     * be created by the user through editor.
     *
     *
     */
    class world {
    public:
        world() = delete;

        /**
         * @brief construct a new world with a specified name associated
         * with it
         */
        world(const std::string& p_name, ref<level_streamer> p_level_streamer)
          : m_name(p_name), m_level_streamer(p_level_streamer) {}

        virtual ~world() = default;

        //! @return the name of world
        [[nodiscard]] std::string name() const { return m_name; }

        /**
        * @brief Retrieves the currently active scene
        */
        ref<scene> current() {
            return m_current;
        }

        template<typename UScene>
        void default_custom_scene(const std::string& p_name, event::bus& p_bus) {
            m_level_streamer->default_scene<UScene>(p_name, p_bus);
        }

        void current(const std::string& p_name) {
            m_current = m_level_streamer->current_scene(p_name);
        }

    private:
        // std::map<std::string, ref<scene>> m_scene_container;
        ref<scene> m_current;
        std::string m_name = "Undefined Tag";
        ref<level_streamer> m_level_streamer=nullptr;
    };
};