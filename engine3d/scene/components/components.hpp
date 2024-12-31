#pragma once
#include <core/application_instance.hpp>
#include <core/geometry/mesh.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/fwd.hpp>
#include <scene/scene_node.hpp>
// #include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine3d{
    /**
     * @note SceneNode
     * @note SceneNode is specifically for referencing components to the scene node they may be part of
     * @note This is something that would be data-oriented specifically rather then having action component/node
     * @note Might actually change the naming from Mesh to MeshMetadata or something
    */
    struct MeshComponent{
        MeshComponent() = default;
        MeshComponent(const std::string& p_Filename) : MeshMetadata(p_Filename) {}

        Mesh MeshMetadata;
        // SceneNode SceneNodeReference;
    };

    struct Transform{
        glm::highp_vec3 Position{0.f};
        glm::highp_vec4 QuaternionRotation{0.f};
        glm::highp_vec3 Rotation{0.f};
        glm::highp_vec3 Scale{0.f};
    };

    class Camera{
        struct CameraData{
            float PerspectiveFOV = glm::radians(45.0f);
            float PerspectiveNear = 0.01;
            float PerspectiveFar = 1000.0f;
            float OrthographicSize = 10.0f;
            float OrthographicNear = 1.0f;
            float OrthographicFar = 1.0f;
        };
        
        enum class ProjectionType{
            UNDEFINED = -1,
            ORTHOGRAPHIC = 0,
            PERSPECTIVE = 1
        };
    public:
        // Camera() = default;
        Camera(){
            m_ViewportSize = {ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight()};
            m_AspectRatio = ApplicationInstance::GetWindow().GetAspectRatio();
            RecalculateView();
        }

        // void SetOrthographicProject(float left, float right, float top, float bottom, float near, float far);

        void SetViewDirection(glm::vec3 p_Position, glm::vec3 Direction, glm::vec3 Up = glm::vec3(0.f, -1.f, 0.f)){
            const glm::vec3 w{glm::normalize(Direction)};
            const glm::vec3 u{glm::normalize(glm::cross(w, Up))};
            const glm::vec3 v{glm::cross(w, u)};

            m_View = glm::mat4{1.f};
            m_View[0][0] = u.x;
            m_View[1][0] = u.y;
            m_View[2][0] = u.z;
            m_View[0][1] = v.x;
            m_View[1][1] = v.y;
            m_View[2][1] = v.z;
            m_View[0][2] = w.x;
            m_View[1][2] = w.y;
            m_View[2][2] = w.z;
            m_View[3][0] = -glm::dot(u, p_Position);
            m_View[3][1] = -glm::dot(v, p_Position);
            m_View[3][2] = -glm::dot(w, p_Position);
        }

        void SetViewTarget(glm::vec3 Position, glm::vec3 Target, glm::vec3 Up = glm::vec3(0.f, -1.f, 0.f)){
            SetViewDirection(Position, Target, Up);
        }

        void SetView(const glm::mat4& p_Matrix){
            m_View = p_Matrix;
        }

        void SetViewXYZ(const glm::vec3& Position, const glm::quat& Rotation){
            glm::vec3 newPos = glm::vec3{Position.x, Position.y - .05f, Position.z};
            const glm::vec3 u = Rotation * glm::vec3(1.0f, 0.0f, 0.0f); // Right vector
            const glm::vec3 v = Rotation * glm::vec3(0.0f, 1.0f, 0.0f); // Up vector
            const glm::vec3 w = Rotation * glm::vec3(0.0f, 0.0f, 1.0f); // Forward vector (negated for view space)

            // Initialize the view matrix
            glm::mat4 viewMatrix = glm::mat4(1.0f);

            // Set the rotation part of the view matrix
            viewMatrix[0][0] = u.x;
            viewMatrix[1][0] = u.y;
            viewMatrix[2][0] = u.z;
            viewMatrix[0][1] = v.x;
            viewMatrix[1][1] = v.y;
            viewMatrix[2][1] = v.z;
            viewMatrix[0][2] = w.x;
            viewMatrix[1][2] = w.y;
            viewMatrix[2][2] = w.z;

            // Set the translation part of the view matrix
            viewMatrix[3][0] = -glm::dot(u, newPos);
            viewMatrix[3][1] = -glm::dot(v, newPos);
            viewMatrix[3][2] = -glm::dot(w, newPos);

            // Assign the resulting view matrix (assuming a class member variable `m_ViewMatrix`)
            m_View = viewMatrix;
        }

        void SetPerspectiveProjection(float fovy, float aspect, float near, float far){
            assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
            const float tanHalfFovy = tan(fovy / 2.f);
            m_Projection = glm::mat4{0.0f};
            m_Projection[0][0] = 1.f / (aspect * tanHalfFovy);
            m_Projection[1][1] = 1.f / (tanHalfFovy);
            m_Projection[2][2] = far / (far - near);
            m_Projection[2][3] = 1.f;
            m_Projection[3][2] = -(far * near) / (far - near);
        }

        glm::quat GetOrientation() const{
            return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 1.0f));
        }

        glm::vec3 UpDirection() const {
            return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::vec3 RightDirection() const {
            return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
        }

        glm::vec3 ForwardDirection() const {
            return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
        }

        glm::mat4 GetProjection() const { return m_Projection; }
        
        glm::mat4 GetView() const { return m_View; }

    private:
        void RecalculateView(){
            m_CurrentPosition = CalculateNewPosition();
            glm::quat orientation = GetOrientation();
            m_View = glm::translate(glm::mat4(1.0f), m_CurrentPosition) * glm::toMat4(orientation);
            m_View = glm::inverse(m_View);
        }

        glm::vec3 CalculateNewPosition() const {
            return m_FocalPoint - ForwardDirection() * m_Distance;
        }

    private:
        std::string m_Tag = "Untitled";
        glm::vec2 m_InitialMousePosition{0.0f, 0.0f};
        // float m_Fov = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.0f, m_FarClip = 1000.0f;
        float m_AspectRatio = 1.778f;
        glm::mat4 m_View;
        glm::mat4 m_Projection;
        glm::vec3 m_CurrentPosition;
        glm::vec3 m_FocalPoint;

        float m_Distance = 10.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;
        glm::vec2 m_ViewportSize;
    };
};