#pragma once
// #include <core/scene/world.hpp>
#include <core/engine_logger.hpp>

namespace atlas{
    /**
     * @name system_registry
     * @note Utilizing service-locator pattern
     * @note Registry for storing world properties
     * @note Enable for scene to grab context from the world they are associated with
     * @note World Scope will act as the global wide scope
     * @note Scene is the local scope player or game objects will be contained in
    */
    class world_scope;
    class system_registry{
    public:

        //! @note We initialize system registry
        static void initialize();

        // template<typename UWorld>
        // static void register_to(world_scope* p_world);
        
        static void register_to(world_scope* p_world);

        // static world_scope* get_world();
        static world_scope get_world();

    private:
        //! @note For now we only support single world_scope creations
        // static world_scope* s_CurrentWorld;
    };
};