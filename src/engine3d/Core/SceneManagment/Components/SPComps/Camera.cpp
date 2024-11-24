// #include "Camera.hpp"
#include <Core/SceneManagment/Components/SPComps/Camera.hpp>
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
    Camera::Camera(const std::string& p_Tag) : m_Tag(p_Tag) {}

    void Camera::OnIntegrate(){
    }

    void Camera::OnUpdate(){
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

    void Camera::SetInitialProperties(float p_Fov, float p_NearClip, float p_FarClip){
        m_Fov = p_Fov;
        m_AspectRatio = ((float)ApplicationInstance::GetWindow().GetWidth() / (float)ApplicationInstance::GetWindow().GetHeight());
        m_NearClip = p_NearClip;
        m_FarClip = p_FarClip;

        m_ViewportSize.x = (float)ApplicationInstance::GetWindow().GetWidth();
        m_ViewportSize.y = (float)ApplicationInstance::GetWindow().GetHeight();
        RecalculateView();
    }

   void Camera::SetViewDirection(glm::vec3 Position, glm::vec3 Direction, glm::vec3 Up){
        const glm::vec3 w{glm::normalize(Direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, Up))};
        const glm::vec3 v{glm::cross(w, u)};

        m_ViewMatrix = glm::mat4{1.f};
        m_ViewMatrix[0][0] = u.x;
        m_ViewMatrix[1][0] = u.y;
        m_ViewMatrix[2][0] = u.z;
        m_ViewMatrix[0][1] = v.x;
        m_ViewMatrix[1][1] = v.y;
        m_ViewMatrix[2][1] = v.z;
        m_ViewMatrix[0][2] = w.x;
        m_ViewMatrix[1][2] = w.y;
        m_ViewMatrix[2][2] = w.z;
        m_ViewMatrix[3][0] = -glm::dot(u, Position);
        m_ViewMatrix[3][1] = -glm::dot(v, Position);
        m_ViewMatrix[3][2] = -glm::dot(w, Position);
   }

    //! @note Used to get the camera to view at a specific target.
    void Camera::SetViewTarget(glm::vec3 Position, glm::vec3 Target, glm::vec3 Up){
        SetViewDirection(Position, Target - Position, Up);
    }

    //! @note Euler angles to specify the rotation of transforms for orienting the camera.
    void Camera::SetViewXYZ(glm::vec3 Position, glm::vec3 Rotation){
        const float c3 = glm::cos(Rotation.z);
        const float s3 = glm::sin(Rotation.z);
        const float c2 = glm::cos(Rotation.x);
        const float s2 = glm::sin(Rotation.x);
        const float c1 = glm::cos(Rotation.y);
        const float s1 = glm::sin(Rotation.y);
        const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
        const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
        const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
        m_ViewMatrix = glm::mat4{1.f};
        m_ViewMatrix[0][0] = u.x;
        m_ViewMatrix[1][0] = u.y;
        m_ViewMatrix[2][0] = u.z;
        m_ViewMatrix[0][1] = v.x;
        m_ViewMatrix[1][1] = v.y;
        m_ViewMatrix[2][1] = v.z;
        m_ViewMatrix[0][2] = w.x;
        m_ViewMatrix[1][2] = w.y;
        m_ViewMatrix[2][2] = w.z;
        m_ViewMatrix[3][0] = -glm::dot(u, Position);
        m_ViewMatrix[3][1] = -glm::dot(v, Position);
        m_ViewMatrix[3][2] = -glm::dot(w, Position);
    }


    void Camera::SetOrthoProjection(float left, float right, float top, float bottom, float near, float far){
        m_ProjectionMatrix = glm::mat4{1.0f};
        m_ProjectionMatrix[0][0] = 2.f / (right - left);
        m_ProjectionMatrix[1][1] = 2.f / (bottom - top);
        m_ProjectionMatrix[2][2] = 1.f / (far - near);
        m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
        m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        m_ProjectionMatrix[3][2] = -near / (far - near);
    }

    
    void Camera::SetPerspectiveProjection(float fovy, float aspect, float near, float far){
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovy / 2.f);
        m_ProjectionMatrix = glm::mat4{0.0f};
        m_ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
        m_ProjectionMatrix[1][1] = -1.f / (tanHalfFovy);
        m_ProjectionMatrix[2][2] = far / (far - near);
        m_ProjectionMatrix[2][3] = 1.f;
        m_ProjectionMatrix[3][2] = -(far * near) / (far - near);
    }

    glm::quat Camera::GetOrientation() const {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 1.0f));
    }

    glm::vec3 Camera::GetUpDirection() const{
        return glm::rotate(GetOrientation(), glm::vec3{0.0f, 1.0f, 0.0f});
    }
    
    glm::vec3 Camera::GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 Camera::GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }


    //! @note Getting pans.
    void Camera::CameraPan(const glm::vec2& p_Data){}

    void Camera::CameraZoom(float delta) {
        // float dt = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime;
        m_Distance -= (delta * ZoomSpeed());

        if(m_Distance < 1.0f){
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    void Camera::CameraRotate(const glm::vec2& p_Position) {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * p_Position.x * RotationSpeed();
		m_Pitch += p_Position.y * RotationSpeed();
    }

    float Camera::RotationSpeed() const {
        return 0.8f;
    }

    glm::vec3 Camera::CalculateNewPosition() const{
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    float Camera::ZoomSpeed() const{
        float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
    }

    void Camera::RecalculateProjection() {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
        RecalculateView();
    }

    void Camera::RecalculateView() {
        m_Position = CalculateNewPosition();
        glm::quat orientation = GetOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }

    glm::vec3 Camera::RecalculatePosition() const {
        return m_FocalPoint * GetForwardDirection() * m_Distance;

    }

    glm::vec2 Camera::PanSpeed() const{
        float x = std::min(m_ViewportSize.x / 1000.0f, 2.4f); // max = 2.4.f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		
		float y = std::min(m_ViewportSize.y / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return {xFactor, yFactor};
    }
};