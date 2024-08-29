#pragma once
#include <Core/internal/Core.h>
#include <engine3d/Scene/Camera.h>

namespace engine3d{
    class OrthographicCamera : public Camera{
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float down, float up);

    private:
        void UpdateViewMatrix();

    private:
        // TransformationMatrix m_TransformationMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        float m_Rotation = 0.0f;

        CameraData m_Data;
    };
};