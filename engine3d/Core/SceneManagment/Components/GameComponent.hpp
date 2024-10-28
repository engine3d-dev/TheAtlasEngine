#pragma once

#include <cstdint>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
class GameComponent
{
    public:
        GameComponent();
        ~GameComponent();

        void SetSceneObjectRef(SceneObject& p_Object);
        void SetComponentID(uint64_t p_CompID);


    private:
        SceneObject* m_GameObjectRef;
        uint64_t m_ComponentID;


};