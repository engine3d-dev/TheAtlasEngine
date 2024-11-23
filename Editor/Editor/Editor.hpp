#pragma once
#include <Core/ApplicationInstance.hpp>
#include "EditorScene.hpp"

namespace engine3d{

    //! @note Our actual editor application
    //! @note TODO -- Having a cenrtal application that will be used across both editor and the game(being developed)
    //! @note So when they're packaged, they can be shipped under one application excluding editor stuff and only things relevant to the game.
    class EditorApplication : public ApplicationInstance{
    public:
        EditorApplication(const std::string& debugName = "Engine3D Editor");
        virtual ~EditorApplication();
        
        //! @note TODO -- Probably have a cleanup handler for this
        //! @note Such as WorldCleanup() or some API to make sure we can cleanly deallocate and delete things...
        // void ShutdownEditor();
        // void UpdateThisApplicationInstance() override;
        void OnApplicationUpdate();
        
    private:
        EditorScene* m_EditorScene;



    };
};