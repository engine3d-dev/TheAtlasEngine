#pragma once
#include <string>
#include <Core/Core.hpp>
#include <Core/API.hpp>
#include <Core/Window.hpp>

namespace engine3d{
    /**
     * @name ApplicationInstance
     * @note Each application(or game) is defined as a separate application
     * @note An application would be the engine3d editor, and games (devs build) are consider separate applications from the engine3d toolkit.
     * @note Defining each of our applications instance.
    */
    // class Window;
    class ApplicationInstance{
    public:
        ApplicationInstance(const std::string&);
        void ExecuteApplicationMainloop();

        //! @note Setting our application name.
        static std::string& CurrentApplicationName();

        static float CurrentFrameTime();

        static ApplicationInstance& Super();

        static void SetCurrentAPI(API p_CurrentApi);

        static Window& GetWindow();

        static API CurrentAPI();
    private:
        //! @note 
        void UpdateCurrentApplicationInstance();
    private:
        virtual void UpdateThisApplicationInstance() = 0;

    private:
        Window* m_Window;
        static ApplicationInstance* g_ThisInstance;
    };

    ApplicationInstance* InitializeApplication();
};


/*




class WorldExample : public engine3d::BasicWorld{
public:

private:
};

class UIOverlay : public engine3d::BaseUIOverlay{

};

class EditorApplication : public engine3d::GameApplicationInstance{
public:

protected:
    void StartApplication() override{}

    void StopApplication() override{}

    // void ApplicationUpdate() override{}

    // Take out UIRender();
    // replace it with the following
    // Option #1
    void ApplicationUpdate() override{
        CurrentApplication()->AddWidget<UIPanel>(&panel);
    }

    // Option #2
    void ApplicationUpdate() override{
        CurrentApplication()->AddOverlay(&panel);
    }

    // Option #3
    void ApplicationUpdate() override{
        // auto& make_ui<UIOverlay(GameInstance* instance, ui);
        auto& error = make_overlay<UIOverlay>(this, ui);
    }

    WorldExample m_CurrentWorld;
};

*/

/*

// Game example


[engine3d::InitializeCore()] -> [Application::Init(), WorldInstance::Init() and UI::Init()] -> [UI-Overlay] -> [WorldInstance] -> []

[engine3d::InitCore()]
- Initializes Engine3D's instances
- Wrapper, Loggers, Abstraction Layer, Editor, and core engine3d module initiation phase.

[Application::Init()]
- Initiates our current application state (Editor or game)
- Setting up our world, actors, objects, UI-related widgets
- Setting up levels of the current world.

[WorldInstance::Init() and UI::Init()]
- Setting up our world objects
- Creating our basic platform, shaders, and other utilities part of the worlds
- Setting up our current scene.

[UI::Init()]
- Used for initiating our UI-framework (imgui)
- Setting up colorschemes, UI layout, widgets, and components of our UI.
- Including setting up how our overlays to be ready to used for creating HUD's.

[WorldInstance]
- Once Engine3D is able to initiate our entire engine.
- Instantiating involves setting up game loop, frames, worlds, levels, actors, world instance, and many more.
- WorldInstance acts as worlds that the end-user developer can use to develop their own world
- Instantiates how our world gets setup preparing the world for adding in actors
- Usage - is when users create a new game. Then in this "WorldInstance" we would create an instance of a world that wouldn be used to build out game.
- WorldInstance acts as a concept in Engine3D for telling how our world is built, how are things interacted such as lighting, objects, models, etc.

class WorldExample : public engine3d::WorldInstance {
public:

private:
    void BeginPlay() override;
    void EndPlay() override;

    void Spawn() override{};

private:
};


class EditorApplication : public engine3d::BasicApplicationState{
public:
    EditorApplication() {
        // What this means is setting our current application that Engine3D will run first.
        // Intention behind this use is implying our current instance that we are running is the editor.
        // Though how do we handle if we have the editor and the game itself?
        // Then we would have a way of differentiating the two.
        BaseApplication::SetCurrentApplication(this);

        // I propose that how we handle instance lifetimes is by doing.
        // Which this means that our intention is to allow the creation of this new game application.
        // Indicating that when we play the game we will run our game instance.

        // [NOTE] - we should control which instance we want to run. For controlling that if we are using the editor we can run specific parts of our instances such as animations, simulations, and various other pieces.
        BaseApplication::SetNewInstance(this);
    }
};

*/