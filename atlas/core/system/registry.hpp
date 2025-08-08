#pragma once
#include <map>
#include <core/scene/world.hpp>

namespace atlas {
    /**
     * @name system_registry
     * @note Utilizing service-locator pattern
     * @note Registry for storing world properties
     * @note Enable for scene to grab context from the world they are associated
     * with
     * @note Manages the lifetimes of world_scope created by the user
     * @note World scope is the container of scenes
     * @note system_registry is allowed to create, search alread-created
     * world_scope's
     */
    class system_registry {
    public:
        system_registry(const std::string& p_tag);

        ~system_registry();

        /**
         * @note system_registry does the following:
         * 1. Creates world scopes
         * 2. Manages Lifetimes of world objects
         * 3. Keeps tracks of the refcounting of references to these world
         * objects
         * 4. Provide globalized access to other worlds
         */

        //! @brief Instantiates new world_scope
        static ref<world_scope> create_world(const std::string& p_tag);

        //! @brief Searches and returns world_scope if found
        //! @brief Returns nullptr if world_scope not found
        static ref<world_scope> get_world(const std::string& p_tag);

    private:
        ref<world_scope> search_world(const std::string& p_tag);

        void append_world(const ref<world_scope>& p_world);

        ref<world_scope> append_world_scope(const ref<world_scope>& p_world);

    private:
        static system_registry* s_instance;
        std::string m_tag = "Undefined";
        std::map<std::string, ref<world_scope>> m_world_registered;
    };
};