#pragma once

#include <Core/SceneManagment/Components/GameComponent.hpp>
#include <glm/glm.hpp>
#include <string>

namespace engine3d{
    class EditorCamera : public GameComponent{
    public:
        struct CameraData{
            float PerspectiveFOV = glm::radians(45.0f);
            float PerspectiveNear = 0.01;
            float PerspectiveFar = 1000.0f;
            float OrthographicSize = 10.0f;
            float OrthographicNear = 1.0f;
            float OrthographicFar = 1.0f;
        };

        enum CameraProjectionType{
            UNDEFINED = -1,
            ORTHOGRAPHIC = 0,
            PERSPECTIVE = 1
        };


        EditorCamera() = default;
        EditorCamera(const std::string& p_Tag);

        void SetInitialProperties(float p_Fov, float p_NearClip, float p_FarClip);
        void OnUpdate();
        
        //! @note Eventually we will refer to OnIntegrate as OnCreate or  OnPlay or something.
        //! @note Come up with a better name for this for accurate API.
        void OnIntegrate();

        glm::quat GetOrientation() const;
        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        glm::mat4 GetProjection() const { return m_ProjectionMatrix; }
        glm::mat4 GetView() const { return m_ViewMatrix; }


        //! @note Getting pans.
        void CameraPan(const glm::vec2& p_Data);
        void CameraZoom(float delta);
        void CameraRotate(const glm::vec2& p_Data);

    private:
        glm::vec3 CalculateNewPosition() const;
        void RecalculateView();

        void RecalculateProjection();

        glm::vec3 RecalculatePosition() const;

        glm::vec2 PanSpeed() const;
        float ZoomSpeed() const;
        
        float RotationSpeed() const;

    private:
        std::string m_Tag;
        glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};
        float m_Fov = 45.0f, m_AspectRatio = 1.778f, m_NearClip=0.1f, m_FarClip = 1000.0f;

        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::vec3 m_Position;
        glm::vec3 m_FocalPoint;
        //! @note Moving this out of Editor Camera.
        // glm::vec2 m_InitialMousePosition
        float m_Distance = 10.0f;
        float m_Pitch = 0.f, m_Yaw = 0.f;
        glm::vec2 m_ViewportSize;

    };
};