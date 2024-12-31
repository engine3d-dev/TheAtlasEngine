#pragma once
#include <string>
#include <glm/glm.hpp>

namespace engine3d{
    /**
     * @name Scene Camera
     * @note Is defined as our camera in a given scene
     * @note There are different types of camera
     * @note I want to define this as an interface because there can be multiple ways of how we define a camera and it's uses
     * @note Meaning if something seems broad enough in the sense like the camera, then it would be an interface
     * 
     * @param TargetCamera
     * @note Camera that has a target that it focuses on a specific target (can be used for camera following by setting a target in the scene)
     * @note Useful for aligning camera with specific scene nodes
     * 
     * @param EditorCamera
     * @note Camera that is within the editor that gets modified within world-space.
     * @note Used for editorial purposes of being modified within a particular 3D environment
     * @note Can switch between the two projection types of Orthographic and Perspective
     * 
     * @param OrthographicCamera
     * @note Where we utilize orthographic projection for the camera
     * @note Meaning orthographic shows objects without perspective distortion
     * 
     * @param PerspectiveCamera
     * @note Is similar to how human eye sees depth , making objects further away appear smaller
     * 
    */
    class SceneCamera{
        enum class ProjectionType { PERSPECTIVE=0, ORTHOGRAPHIC=1 };
    public:
        virtual ~SceneCamera() = default;

        void OnCreate(){ return Create(); }

        void OnStart() { return Start(); }
        
        void OnUpdate() { return Update(); }

        void OnStop() { return Stop(); }

    private:
        virtual void Create() = 0;
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void Stop() = 0;
    };


    // class EditorCamera : public SceneCamera{
    // public:
    //     EditorCamera() = default;
    //     EditorCamera(float p_Fov, float p_NearClip, float p_FarClip);
    //     virtual ~EditorCamera() {}

    //     // When the viewport get's resized we want our editor camera to be able in handling that resize event
    //     void OnResize(uint32_t p_Width, uint32_t p_Height){
    //         if(p_Width){}
    //         if(p_Height){}
    //     }

    //     void SetTag(const std::string& p_Tag){
    //         if(p_Tag.empty()){}
    //     }

    //     float GetRotationSpeed() const;
    //     void SetRotationSpeed(float p_Speed);

    //     float GetZoomSpeed() const;


    //     void SetForwardDirection(glm::vec3 p_ForwardDirection);
    //     void SetRightDirection(glm::vec3 p_RightDirection);
    //     void SetUpDirection(glm::vec3 p_UpDirection);


    //     // glm::vec3 ForwardDirection() const;
    //     // glm::vec3 RightDirection() const;
    //     // glm::vec3 UpDirection() const;

    //     float GetPitch() const;
    //     float Getyaw() const;

    // private:
    //     void Create() override {
    //         //! @note This would get set in the .cpp
    //         m_ViewportSize = {0.f, 0.f};
    //     }
    //     void Start() override {}
    //     void Update() override {}
    //     void Stop() override {}

    // private:
    //     std::string m_Tag="Untitled";
    //     glm::mat4 m_Projection;
    //     glm::mat4 m_View;
    //     glm::vec3 m_Position;
    //     glm::vec3 m_FocalPoint;
    //     glm::vec3 m_Rotation;

    //     // constraints for the editor camera, specifically.
    //     float m_Distance{10.0f};
    //     float m_Pitch{0.0f}, m_Yaw{0.0f};
    //     glm::vec2 m_ViewportSize;
    // };
};