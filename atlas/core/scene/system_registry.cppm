module;

#include <map>
#include <string>

export module atlas.core.scene.system_registry;

import atlas.common;
import atlas.core.scene.world;

export namespace atlas {
    /**
     * @brief system registry acts as a utility for managing creation of game
     * worlds initially
     *
     * Initially utilized to manage lifetimes of worlds and allow for by default
     * setting to world #0, where we use few functions to get access to specific
     * contexts
     *
     *
     * TODO: This is going to be replaced with a level streaming manager that
     * will help manage, maintain, and default creationg of a world and a scene.
     * With additional responsibilities in how the world context gets created.
     */
    class system_registry {
    public:
        /**
         * @brief construct a new system registry
         *
         * @param p_name is for specifying a name to give this registry
         */
        system_registry(const std::string& p_name)
          : m_tag(p_name) {

            s_instance = this;
        }

        ~system_registry() = default;

        /**
         * @note system_registry does the following:
         * 1. Creates world scopes
         * 2. Manages Lifetimes of world objects
         * 3. Keeps tracks of the refcounting of references to these world
         * objects
         * 4. Provide globalized access to other worlds
         */

        /**
         * @brief constructs a new world
         *
         * Initially this was used to create a world with a specified name
         * associated with it
         *
         * This was used for getting world to be maintained implicitly by
         * system_registry, though this will be changing.
         */
        static ref<world> create_world(const std::string& p_tag) {
            return s_instance->append_world_and_get(create_ref<world>(p_tag));
        }

        //! @brief Searches and returns world if found
        //! @brief Returns nullptr if world not found

        /**
         * @brief searches in getting the world and looks up based on its
         * specified name
         *
         * @return nullptr if not found, otherwise return
         * shared_ptr<world>
         */
        static ref<world> get_world(const std::string& p_tag) {
            return s_instance->m_world_registered[p_tag];
        }

    private:
        ref<world> search_world(const std::string& p_tag) {
            return m_world_registered[p_tag];
        }

        // void append_world(const ref<world>& p_world);

        ref<world> append_world_and_get(const ref<world>& p_world) {
            m_world_registered.emplace(p_world->name(), p_world);
            return m_world_registered[p_world->name()];
        }

    private:
        static system_registry* s_instance;
        std::string m_tag = "Undefined";
        std::map<std::string, ref<world>> m_world_registered;
    };

    system_registry* system_registry::s_instance = nullptr;
};