#pragma once
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace engine3d{
    /**
     * @name Scene
     * @note Defines what our scene is and what are our objects in this scene
    */
    class Scene{
    public:
        Scene();

        //! @note OnStart explicitly tells when we want to start our scene
        //! @note Same applies with OnStop
        void OnStart();
        void OnStop();

        //! TODO: This is to setup environment maps (like skybox's or cubemaps in the future)
        // void SetEnvironmentMap(const std::string p_Filename);

        entt::registry& Registry() { return m_Registry; }

    private:
        entt::registry m_Registry;
    };
};



/*


---------------------------------------
A lot of the time engines implement controllers this way
---------------------------------------


class PlayerController : public Entity{
};






*/