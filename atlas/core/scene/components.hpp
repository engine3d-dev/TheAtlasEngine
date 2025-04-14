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
    struct tag {
        std::string TagMetadata = "";
    };

    struct transform {
        glm::highp_vec3 Position{ 0.f };
        glm::highp_vec3 QuaternionRotation{ 0.f };
        glm::highp_vec3 Rotation{ 0.f };
        glm::highp_vec3 Scale{ 1.f };
        glm::highp_vec4 Color{ 1.f };
    };

    //! @note Our interpretation of the RigidBody3D
    struct rigidbody3d {
        rigidbody3d() = default;

        enum class BodyType { STATIC = 0, DYNAMIC = 1, KINEMATIC = 2 };

        BodyType Type = BodyType::STATIC;
        bool HasFixation = false;
        void* BodyAtRuntime =
          nullptr; // for storing the actual data of the body
    };

    //! @note Our interpretation of the BoxCollider that will get interpreted
    //! based on physics API's enabled
    struct box_collider3d {
        box_collider3d() = default;
        box_collider3d(const box_collider3d&) = default;

        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.5f;

        glm::vec3 offset = { 0.f, 0.f, 0.f };
        glm::vec3 size = { 0.5f, 0.5f, 0.5f };
    };

    // Defines several possible options for camera movement. Used as abstraction
    // to stay away from window-system specific input methods
    enum CameraMovement { Forward, Backward, Left, Right, Up, Down };

    // An abstract camera class that processes input and calculates the
    // corresponding Euler Angles, Vectors and Matrices for use in OpenGL

    //! TODO: Make this better (when we do lighting)
    struct light {
        glm::vec3 Position{ 1.f };
    };

    //! @note Contains our render target as the mesh
    //! TODO: IMPORTANT: NEED TO CHANGE HOW THIS WORKSS!!
    //! IMPORTANT: flecs gives you a lifetime issue if it doesnt have a default
    //! constructor implictly added to a struct/class
    struct rendertarget3d {
        rendertarget3d() = default;
        rendertarget3d(const std::string& p_filepath)
          : Filepath(p_filepath)
          , MeshMetaData(p_filepath) {}

        std::string Filepath;
        mesh MeshMetaData{};
        glm::mat4 Model{ 1.f };
    };

    // An abstract camera class that processes input and calculates the
    // corresponding Euler Angles, Vectors and Matrices for use in OpenGL

    /*!
       TODO: Camera class needs to strip out the following properties

       - Zoom

       TODO: Camera should contain

       - Position


    */
    class camera {
        // Default camera values
        // const float yaw = -90.0f;
        // const float PITCH = 0.0f;
        // const float ZOOM = 45.0f;
    public:
        // constructor with vectors
        camera(glm::vec3 position = glm::vec3(0.0f, 1.50f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f),
               float yaw = -90.0f,
               float pitch = 0.0f)
          : MovementSpeed(5.f)
          , MouseSensitivity(0.1f)
          , Zoom(45.0f)
          , camera_mouse_sensitivity(0.1f) {
            Position = position;
            WorldUp = up;
            EulerRotation = { yaw, pitch, 1.f };
            AspectRatio = (float)application::get_aspect_ratio();
            update_camera();
        }

        // returns the view matrix calculated using Euler Angles and the LookAt
        // Matrix
        [[nodiscard]] glm::mat4 get_view() const { return View; }
        [[nodiscard]] glm::mat4 get_projection() const { return Projection; }

        // processes input received from any keyboard-like input system. Accepts
        // input parameter in the form of camera defined ENUM (to abstract it
        // from windowing systems)
        void process_keyboard(CameraMovement p_direction, float p_delta_time) {
            float velocity = MovementSpeed * p_delta_time;

            if (p_direction == CameraMovement::Forward)
                Position += get_front() * velocity;
            if (p_direction == CameraMovement::Backward)
                Position -= get_front() * velocity;
            if (p_direction == CameraMovement::Left)
                Position -= Right * velocity;
            if (p_direction == CameraMovement::Right)
                Position += Right * velocity;

            if (p_direction == CameraMovement::Up) {
                Position += Up * velocity;
            }

            if (p_direction == CameraMovement::Down) {
                Position -= Up * velocity;
            }
        }

        // processes input received from a mouse input system. Expects the
        // offset value in both the x and y direction.
        void process_mouse_movement(float p_x,
                                    float p_y,
                                    bool p_constraint_pitch = true) {

            p_x *= MouseSensitivity;
            p_y *= MouseSensitivity;

            EulerRotation.x += p_x;
            EulerRotation.y += p_y;

            // make sure that when pitch is out of bounds, screen doesn't get
            // flipped
            if (p_constraint_pitch) {
                if (EulerRotation.y > 89.0f) {
                    EulerRotation.y = 89.0f;
                }
                if (EulerRotation.y < -89.0f) {
                    EulerRotation.y = -89.0f;
                }
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            update_camera();
        }

        // processes input received from a mouse scroll-wheel event. Only
        // requires input on the vertical wheel-axis
        void process_mouse_scroll(float yoffset) {
            Zoom -= (float)yoffset;

            if (Zoom < 1.0f) {
                Zoom = 1.0f;
            }

            if (Zoom > 45.0f) {
                Zoom = 45.0f;
            }
        }

        //! TODO: REMOVE THESE -- these should be user-defined
        void set_movement_speed(float p_sensitivity) {
            camera_movement_sensitivity = p_sensitivity;
            MovementSpeed = camera_movement_sensitivity;
        }

        void set_mouse_speed(float p_sensitivity) {
            camera_mouse_sensitivity = p_sensitivity;
        }

        [[nodiscard]] float camera_sensitivity() const {
            return camera_mouse_sensitivity;
        }

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void update_camera() {
            // calculate the new Front vector
            // glm::vec3 front;
            // front.x = cos(glm::radians(EulerRotation.x)) *
            // cos(glm::radians(EulerRotation.y)); front.y =
            // sin(glm::radians(EulerRotation.y)); front.z =
            // sin(glm::radians(EulerRotation.x)) *
            // cos(glm::radians(EulerRotation.y)); Front =
            // glm::normalize(front); also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(
              get_front(),
              WorldUp)); // normalize the vectors, because their length
                         // gets closer to 0 the more you look up or
                         // down which results in slower movement.
            Left = glm::normalize(glm::cross(-get_front(), WorldUp));
            Up = glm::normalize(glm::cross(Right, get_front()));
            Down = glm::normalize(glm::cross(-Right, WorldUp));
        }

    public:
        void update_proj_view() {
            //! TODO: Eventually we will have camera configurations that will
            //! utilize this.
            Projection =
              glm::perspective(glm::radians(Zoom), AspectRatio, .1f, 50000.f);
            View = glm::lookAt(Position, Position + get_front(), Up);
        }

        [[nodiscard]] glm::vec3 get_front() const {
            glm::vec3 front_values;
            front_values.x = cos(glm::radians(EulerRotation.x)) *
                             cos(glm::radians(EulerRotation.y));
            front_values.y = sin(glm::radians(EulerRotation.y));
            front_values.z = sin(glm::radians(EulerRotation.x)) *
                             cos(glm::radians(EulerRotation.y));
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
        float MovementSpeed{};
        float MouseSensitivity{};
        float Zoom{};

        // float camera_mouse_sensitivity = 0.1f;
        float camera_mouse_sensitivity = 2.5f;
        float camera_movement_sensitivity = 2.5f;

        // toggling between cameras and checking if our current
        bool IsMainCamera = false;
    };
}; // namespace atlas