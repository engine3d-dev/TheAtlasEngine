#pragma once
#include <core/engine_logger.hpp>
#include <map>
#include <core/scene/world.hpp>

namespace atlas {
    /**
     * @name system_registry
     * @note Utilizing service-locator pattern
     * @note Registry for storing world properties
     * @note Enable for scene to grab context from the world they are associated
     * with
     * @note World Scope will act as the global wide scope
     * @note Scene is the local scope player or game objects will be contained
     * in
     */
    class world_scope;
    class system_registry {
    public:
        system_registry(const std::string& p_tag);
        
        ~system_registry() {
            console_log_fatal("~system_registry called!!!");
        }


        /**
         * @note system_registry does the following:
         * 1. Creates world scopes
         * 2. Manages Lifetimes of world objects
         * 3. Keeps tracks of the refcounting of references to these world objects
         * 4. Provide globalized access to other worlds
        */

        static ref<world_scope> create_world(const std::string& p_tag);

        //! @brief We should be able to register multiple worlds
        //! @brief While also selecting which world is the player's current world they're located in
        static void register_to(const ref<world_scope>& p_world);

        // static world_scope* get_world();
        static world_scope get_world();

        static ref<world_scope> get_world(const std::string& p_tag);


        //! @brief Iteration #2 on registering world
        // static void register_world()

    private:

        ref<world_scope> search_world(const std::string& p_tag);

        void append_world(const ref<world_scope>& p_world) {
            // m_initial_worlds.push_back(p_world);
            if(p_world == nullptr) {
                console_log_fatal("p_world = nullptr!!!");
                return;
            }

            m_world_registered.insert({p_world->get_tag(), p_world});
            console_log_fatal("After inserting p_world->tag = {}", p_world->get_tag());
        }

        ref<world_scope> append_world_scope(const ref<world_scope>& p_world) {
            if(p_world == nullptr) {
                return nullptr;
            }

            m_world_registered.insert({p_world->get_tag(), p_world});
            return m_world_registered[p_world->get_tag()];
        }

    private:
        static system_registry* s_instance;
        std::string m_tag = "Undefined";
        // std::deque<world_scope*> m_initial_worlds;
        std::map<std::string, ref<world_scope>> m_world_registered;
    };
};