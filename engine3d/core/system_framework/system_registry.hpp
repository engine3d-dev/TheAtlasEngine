#pragma once
// #include <core/scene/world.hpp>
#include <core/engine_logger.hpp>

namespace engine3d{
    /**
     * @name SystemRegistry
     * @note Utilizing service-locator pattern
     * @note Registry for storing world properties
     * @note Enable for scene to grab context from the world they are associated with
     * @note World Scope will act as the global wide scope
     * @note Scene is the local scope player or game objects will be contained in
    */
    class WorldScope;
    class SystemRegistry{
    public:

        //! @note We initialize system registry
        static void InitializeRegistry();

        // template<typename UWorld>
        // static void Register(WorldScope* p_World);
        
        static void Register(WorldScope* p_World);

        // static WorldScope* GetWorld();
        static WorldScope GetWorld();

    private:
        //! @note For now we only support single worldScope creations
        // static WorldScope* s_CurrentWorld;
    };
};