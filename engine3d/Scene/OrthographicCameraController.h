#pragma once
#include <engine3d/Core/internal/Core.h>
#include <engine3d/Event/Event.h>
#include <engine3d/Core/Timestep.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace engine3d{
    class OrthographicCamera{
    public:
        OrthographicCamera(float left, float right, float bottom, float top) : m_Projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_View(1.0f) {
            m_ViewAndProjection = m_Projection * m_View;
            m_Width = (right - left);
            m_Height = (top - bottom);
        }

        void SetProjection(float left, float right, float bottom, float top){
            m_Projection = glm::ortho(left, right, bottom, top);
            m_ViewAndProjection = m_Projection * m_View;
        }

        float GetWidth() const { return m_Width; }
        float GetHeight() const { return m_Height; }

        const glm::vec3 GetPosition() const { return m_Position; }

        float GetRotation() const { return m_Rotation; }

        void SetPosition(const glm::vec3& position){}

        void SetRotation(float rotation){}


        const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
        const glm::mat4& GetViewMatrix() const { return m_View; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewAndProjection; }

    private:
        void UpdateViewMatrix(){
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
                                  * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

            m_View = glm::inverse(transform);
            m_ViewAndProjection = m_Projection * m_View;
        }
    
    private:
        glm::mat4 m_View, m_Projection, m_ViewAndProjection;
        glm::vec3 m_Position;
        float m_Width, m_Height;
        float m_Rotation = 0.f;
    };

    class OrthographicCameraController{
    public:
        // OrthographicCameraController(bool rotation=false);

        // void OnEvent(Event& event);
        // void OnUpdate(Timestep ts);
        // void OnResize(float w, float h);

        // OrthographicCamera& GetCamera();
        // OrthographicCamera GetCamera() const;

        // glm::mat4 GetViewMatrix() const;
        // glm::mat4 GetProjectionMatrix() const;

        // void SetZoomLevel(float level);
        // float GetZoomLevel() const;

        OrthographicCameraController();

        void OnEvent(Event& event);
        void OnUpdate(Timestep ts);

        void OnMouseUpdate();

        void OnMouseScroll(double, double);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;


        void OnResize(float width, float height);

        void SetPosition(const glm::vec3& position);
        void SetRotation(float rotationAngle);
    private:

        // void OnUpdateView();

    private:
        // OrthographicCamera m_OrthographicCamera;
        // glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
        // float m_AspectRatio;
        // float m_CameraSpeed;
        // float m_ZoomLevel = 1.0f;
        // bool m_IsRotationEnabled;
        // float m_CameraRotation = 0.f;
        // float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;


        glm::mat4 m_ViewMatrix;
        float m_AspectRatio;
        float m_CameraSpeed;
        float m_Yaw, m_Pitch;
        glm::vec2 m_LastCoords;
        float m_Fov;
        bool m_FirstMouse;
        glm::vec3 m_CameraPosition, m_CameraFront, m_CameraUp;
    };
};;