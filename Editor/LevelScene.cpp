#include "LevelScene.hpp"
#include <core/update_handlers/sync_update.hpp>
#include <core/event/input_poll.hpp>
#include <renderer/renderer.hpp>

namespace engine3d{
    LevelScene::LevelScene(const std::string& p_Tag) : SceneScope(p_Tag){
        //! @note Creating our objects from our scene
        m_Sphere = this->CreateNewObject("sphere");
        Transform sphere_transform;
        sphere_transform.Position = {0.f, 0.f, 0.f};
        sphere_transform.Scale = {.1f,.1f, .1f};
        m_Sphere->AddComponent<Transform>(sphere_transform);
        MeshComponent comp("3d_models/tutorial/sphere.obj");
        m_Sphere->AddComponent<MeshComponent>(comp);

        Transform camera_transform;
        camera_transform.Position = {-1.f, 5.f, -15.f};
        m_Camera = this->CreateNewObject("camera");
        m_Camera->AddComponent<Transform>(camera_transform);
        m_Camera->AddComponent<PerspectiveCamera>();

    }

    void LevelScene::Update() {
        // ConsoleLogInfo("LevelScene::OnUpdate called!");
        auto transform = *m_Camera->GetComponent<engine3d::Transform>();

        auto comp = *m_Camera->GetComponent<engine3d::PerspectiveCamera>();
        // const engine3d::PerspectiveCamera* perspective_camera = m_Camera->GetComponent<engine3d::PerspectiveCamera>();

        float deltaTime = SyncUpdate::DeltaTime();
        
        if (InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
            // glfwSetWindowShouldClose(window, true);
            ApplicationInstance::GetWindow().Close();
        }

        if (InputPoll::IsKeyPressed(ENGINE_KEY_W)){
            // ConsoleLogWarn("W Pressed!");
            comp.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (InputPoll::IsKeyPressed(ENGINE_KEY_S)){
            // ConsoleLogWarn("S Pressed!");
            comp.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (InputPoll::IsKeyPressed(ENGINE_KEY_A)){
            // ConsoleLogWarn("A Pressed!");
            comp.ProcessKeyboard(LEFT, deltaTime);
        }
        if (InputPoll::IsKeyPressed(ENGINE_KEY_D)){
            // ConsoleLogWarn("D Pressed!");
            comp.ProcessKeyboard(RIGHT, deltaTime);
        }
        if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)){
            // UP
            // ConsoleLogWarn("Q Pressed!");
            comp.ProcessKeyboard(UP, deltaTime);
        }
        if(InputPoll::IsKeyPressed(ENGINE_KEY_E)){
            // DOWN
            // ConsoleLogWarn("E Pressed!");
            comp.ProcessKeyboard(DOWN, deltaTime);
        }

        //! @note Press shift key to move using the mouse to rotate around
        if(InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_SHIFT)){
            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_RIGHT)){
                double xPosIn, yPosIn;
                glfwGetCursorPos(ApplicationInstance::GetWindow().GetNativeWindow(), &xPosIn, &yPosIn);

                float x_offset = xPosIn;
                float velocity = x_offset * deltaTime;
                comp.ProcessMouseMovement(velocity, 0.f);
            }

            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_LEFT)){
                double xPosIn, yPosIn;
                glfwGetCursorPos(ApplicationInstance::GetWindow().GetNativeWindow(), &xPosIn, &yPosIn);

                float x_offset = xPosIn;
                float velocity = x_offset * deltaTime;
                comp.ProcessMouseMovement(-velocity, 0.f);
            }

            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_MIDDLE)){
                double xPosIn, yPosIn;
                glfwGetCursorPos(ApplicationInstance::GetWindow().GetNativeWindow(), &xPosIn, &yPosIn);
                float velocity = yPosIn * deltaTime;
                comp.ProcessMouseMovement(0.f, velocity);
            }

            if(InputPoll::IsKeyPressed(ENGINE_KEY_SPACE)){
                double xPosIn, yPosIn;
                glfwGetCursorPos(ApplicationInstance::GetWindow().GetNativeWindow(), &xPosIn, &yPosIn);
                float velocity = yPosIn * deltaTime;
                comp.ProcessMouseMovement(0.f, -velocity);
            }


        }

        transform.Position.x = comp.Position.x;
        transform.Position.y = comp.Position.y;
        transform.Position.z = comp.Position.z;

        m_Camera->SetComponent<engine3d::Transform>(transform);


        // ConsoleLogTrace("transform.Pos.x = {}, transform.Pos.y = {}, transform.Pos.z = {}", transform.Position.x, transform.Position.y, transform.Position.z);
        // comp.SetViewXYZ(my_camera_transform.Position, my_camera_transform.Rotation);
        
        // comp.SetPerspectiveProjection(glm::radians(50.f), ApplicationInstance::GetAspectRatio(), 1.f, 1000.f);
        // transform.Position = comp.Position;
        Renderer::RenderWithCamera(m_Sphere, m_Camera);
    }
};