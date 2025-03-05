#pragma once
#include <core/application.hpp>
#include <core/geometry/mesh.hpp>
#include <glm/ext/quaternion_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace atlas {
    struct Tag {
        std::string TagMetadata = "";
    };

    struct Transform {
        glm::highp_vec3 Position{ 0.f };
        glm::highp_vec3 QuaternionRotation{ 0.f };
        glm::highp_vec3 Rotation{ 0.f };
        glm::highp_vec3 Scale{ 1.f };
        glm::highp_vec4 Color{ 1.f };
    };

    //! @note Our interpretation of the RigidBody3D
    struct RigidBody3D {
        RigidBody3D() = default;

        enum class BodyType { STATIC = 0, DYNAMIC = 1, KINEMATIC = 2 };

        BodyType Type = BodyType::STATIC;
        bool HasFixation = false;
        void* BodyAtRuntime =
          nullptr; // for storing the actual data of the body
    };

    //! @note Our interpretation of the BoxCollider that will get interpreted
    //! based on physics API's enabled
    struct BoxCollider3D {
        BoxCollider3D() = default;
        BoxCollider3D(const BoxCollider3D&) = default;

        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.5f;

        glm::vec3 offset = { 0.f, 0.f, 0.f };
        glm::vec3 size = { 0.5f, 0.5f, 0.5f };
    };

    // Defines several possible options for camera movement. Used as abstraction
    // to stay away from window-system specific input methods
    enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

    // Default camera values
    const float YAW = -90.0f;
    // const float YAW = 45.0f;
    const float PITCH = 0.0f;
    // const float SPEED       =  2.5f;
    // const float SENSITIVITY =  0.1f;
    const float ZOOM = 45.0f;

    // An abstract camera class that processes input and calculates the
    // corresponding Euler Angles, Vectors and Matrices for use in OpenGL

    //! TODO: Make this better (when we do lighting)
    struct Light {
        glm::vec3 Position{1.f};
    };


    //! @note Contains our render target as the mesh
    //! TODO: IMPORTANT: NEED TO CHANGE HOW THIS WORKSS!!
    //! IMPORTANT: flecs gives you a lifetime issue if it doesnt have a default constructor implictly added to a struct/class
    struct RenderTarget3D{
        RenderTarget3D() = default;
        RenderTarget3D(const std::string& p_filepath) : Filepath(p_filepath), MeshMetaData(p_filepath) {}
        
        std::string Filepath;
        Mesh MeshMetaData{};
        glm::mat4 Model{1.f};
    };

    // An abstract camera class that processes input and calculates the
    // corresponding Euler Angles, Vectors and Matrices for use in OpenGL

    /*!
       TODO: Camera class needs to strip out the following properties
       
       - Zoom

       TODO: Camera should contain
       
       - Position

    
    */
    class Camera {
    public:
        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 1.50f, 0.0f),
                            glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f),
                            float yaw = YAW,
                            float pitch = PITCH)
            : MovementSpeed(5.f)
            , MouseSensitivity(0.1f)
            , Zoom(ZOOM)
            , camera_mouse_sensitivity(0.1f) {
            Position = position;
            WorldUp = up;
            EulerRotation = {yaw, pitch, 1.f};
            AspectRatio = application::get_aspect_ratio();
            updateCameraVectors();
        }

        // returns the view matrix calculated using Euler Angles and the LookAt
        // Matrix
        glm::mat4 get_view() const { return View; }
        glm::mat4 get_projection() const { return Projection; }

        // processes input received from any keyboard-like input system. Accepts
        // input parameter in the form of camera defined ENUM (to abstract it
        // from windowing systems)
        void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
            float velocity = MovementSpeed * deltaTime;

            if (direction == FORWARD)
                Position += get_front() * velocity;
            if (direction == BACKWARD)
                Position -= get_front() * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;

            if (direction == UP) {
                Position += Up * velocity;
            }

            if (direction == DOWN) {
                Position -= Up * velocity;
            }
        }

        // processes input received from a mouse input system. Expects the
        // offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset,
                                    float yoffset,
                                    bool constrainPitch = true) {
            
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            EulerRotation.x += xoffset;
            EulerRotation.y += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get
            // flipped
            if (constrainPitch) {
                if (EulerRotation.y > 89.0f){
                    EulerRotation.y = 89.0f;
                }
                if (EulerRotation.y < -89.0f){
                    EulerRotation.y = -89.0f;
                }
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        // processes input received from a mouse scroll-wheel event. Only
        // requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset) {
            Zoom -= (float)yoffset;
            
            if (Zoom < 1.0f){
                Zoom = 1.0f;
            }

            if (Zoom > 45.0f){
                Zoom = 45.0f;
            }
        }

        //! TODO: REMOVE THESE -- these should be user-defined
        void SetCameraMovementSpeed(float Sensitivity) {
            camera_movement_sensitivity = Sensitivity;
            MovementSpeed = camera_movement_sensitivity;
        }

        void SetCameraMouseSpeed(float Sensitivity) {
            camera_mouse_sensitivity = Sensitivity;
        }

        float GetCameraSensitivity() const { return camera_mouse_sensitivity; }

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors() {
            // calculate the new Front vector
            // glm::vec3 front;
            // front.x = cos(glm::radians(EulerRotation.x)) * cos(glm::radians(EulerRotation.y));
            // front.y = sin(glm::radians(EulerRotation.y));
            // front.z = sin(glm::radians(EulerRotation.x)) * cos(glm::radians(EulerRotation.y));
            // Front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(
                get_front(), WorldUp)); // normalize the vectors, because their length
                                // gets closer to 0 the more you look up or
                                // down which results in slower movement.
            Left = glm::normalize(glm::cross(-get_front(), WorldUp));
            Up = glm::normalize(glm::cross(Right, get_front()));
            Down = glm::normalize(glm::cross(-Right, WorldUp));
        }

    public:
        void UpdateProjView() {
            //! TODO: Eventually we will have camera configurations that will
            //! utilize this.
            Projection =
                glm::perspective(glm::radians(Zoom), AspectRatio, .1f, 50000.f);
            View = glm::lookAt(Position, Position + get_front(), Up);
        }

        glm::vec3 get_front() const {
            glm::vec3 front_values;
            front_values.x = cos(glm::radians(EulerRotation.x)) * cos(glm::radians(EulerRotation.y));
            front_values.y = sin(glm::radians(EulerRotation.y));
            front_values.z = sin(glm::radians(EulerRotation.x)) * cos(glm::radians(EulerRotation.y));
            return glm::normalize(front_values);
        }

    public:
        // camera Attributes
        glm::vec3 Position;
        // glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Down;
        glm::vec3 Right;
        glm::vec3 Left;
        glm::vec3 WorldUp;

        glm::mat4 Projection;
        glm::mat4 View;

        float AspectRatio = 0.f;

        // euler Angles
        // {x: Yaw, y: Pitch, z: Roll}
        glm::vec3 EulerRotation;
        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // float camera_mouse_sensitivity = 0.1f;
        float camera_mouse_sensitivity = 2.5f;
        float camera_movement_sensitivity = 2.5f;

        // toggling between cameras and checking if our current
        bool IsMainCamera=false;
    };
}; // namespace atlas