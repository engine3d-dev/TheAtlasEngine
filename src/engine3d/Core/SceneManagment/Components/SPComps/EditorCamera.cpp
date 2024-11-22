// #include "EditorCamera.hpp"
#include <Core/SceneManagment/Components/SPComps/EditorCamera.hpp>
#include <Core/Event/InputPoll.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Core/ApplicationInstance.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine3d{
    EditorCamera::EditorCamera(const std::string& p_Tag) : m_Tag(p_Tag) {}

    void EditorCamera::OnIntegrate(){
    }

    void EditorCamera::OnUpdate(){
        if(InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_ALT)){
            glm::vec2 delta = (InputPoll::GetMousePosition() - m_InitialMousePosition) * 0.003f;
            m_InitialMousePosition = InputPoll::GetMousePosition();

            if(InputPoll::IsMousePressed(Mouse::ButtonMiddle)){
                CameraPan(delta);
            }
            else if(InputPoll::IsMousePressed(Mouse::ButtonLast)){
                CameraRotate(delta);
            }
            else if(InputPoll::IsMousePressed(Mouse::ButtonRight)){
                CameraZoom(delta.y);
            }
        }

        RecalculateView();
    }

    void EditorCamera::SetInitialProperties(float p_Fov, float p_NearClip, float p_FarClip){
        m_Fov = p_Fov;
        m_AspectRatio = ((float)ApplicationInstance::GetWindow().GetWidth() / (float)ApplicationInstance::GetWindow().GetHeight());
        m_NearClip = p_NearClip;
        m_FarClip = p_FarClip;

        m_ViewportSize.x = (float)ApplicationInstance::GetWindow().GetWidth();
        m_ViewportSize.y = (float)ApplicationInstance::GetWindow().GetHeight();
        RecalculateView();
    }

    glm::quat EditorCamera::GetOrientation() const {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 1.0f));
    }

    glm::vec3 EditorCamera::GetUpDirection() const{
        return glm::rotate(GetOrientation(), glm::vec3{0.0f, 1.0f, 0.0f});
    }
    
    glm::vec3 EditorCamera::GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }


    //! @note Getting pans.
    void EditorCamera::CameraPan(const glm::vec2& p_Data){}

    void EditorCamera::CameraZoom(float delta) {
        // float dt = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime;
        m_Distance -= (delta * ZoomSpeed());

        if(m_Distance < 1.0f){
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    void EditorCamera::CameraRotate(const glm::vec2& p_Position) {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * p_Position.x * RotationSpeed();
		m_Pitch += p_Position.y * RotationSpeed();
    }

    float EditorCamera::RotationSpeed() const {
        return 0.8f;
    }

    glm::vec3 EditorCamera::CalculateNewPosition() const{
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    float EditorCamera::ZoomSpeed() const{
        float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
    }

    void EditorCamera::RecalculateProjection() {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
        RecalculateView();
    }

    void EditorCamera::RecalculateView() {
        m_Position = CalculateNewPosition();
        glm::quat orientation = GetOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }

    glm::vec3 EditorCamera::RecalculatePosition() const {
        return m_FocalPoint * GetForwardDirection() * m_Distance;

    }

    glm::vec2 EditorCamera::PanSpeed() const{
        float x = std::min(m_ViewportSize.x / 1000.0f, 2.4f); // max = 2.4.f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		
		float y = std::min(m_ViewportSize.y / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return {xFactor, yFactor};
    }
};