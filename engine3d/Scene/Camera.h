#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Event/Event.h>
#include <engine3d/Core/Timestep.h>
#include "glm/glm.hpp"

namespace engine3d{
    /**
     * @name Camera
     * @note There are a few types of camera, Editor, World, Scene, and Camera Component
     * @note Camera location is based off translation, rotation, and scaling matrices
     * @note Where transformation matrix is built off of the three (translation, rotation, and scaling)
     * 
     * @param EditorCamera
     * @note Our editor camera cannot be changed by the client, but is our actual camera specifically for the editor
     * @note Cannot be modified, or done with. Used inside the editor's runtime.
     * 
     * @param WorldCamera
     * @note Still thinking this will be our environment world
     * @note Way in which we interact and view our world
     * @note Potentially having this represent our scene camera
     * @note When giving client-side access you would do:
     * @example
     * auto camera = Actor::GetWorldCamera();
     * 
     * @param SceneCamera
     * @note This camera would be used for our in-scene gameplay camera
     * @note Users should be able to utilize our scene cameras to place where our players fov, and other viewpoints in our world
     * 
     * @param CameraComponent
     * @note This camera will be a component that can be attached to a player/object at any point in our world
     * @note An example of this is applying a camera component in a game's security camera, or attaching it as a component to our entity
    */

    /**
     * @name Camera Operation
     * @note Should consist of up, down, forward, backward directions
     * @note While also consist of yaw, pitch, near and far clip, focal point, and pan-speed.
     * @note Should allow for sharpness, but more flexibility in how users interact with the camera.
     * 
     * @param Yaw
     * @note rotation around central vertical (normal) axis
     * 
     * @param Pitch
     * @note rotation around central horizontal (lateral) axis
     * 
     * @param NearClip
     * @note objects are rendered at further distance
     * 
     * @param FarClip
     * @note Objects are rendered at closer distance
     * 
     * @param FocalPoint
     * @note sharpness and depth of the field.
     * 
     * @param PanSpeed
     * @note Speed of camera pan
     * @note Camera Pan is which horizontal movement which camera moves right to left (or vice versa) from central axis.
    */
    struct CameraData{
        //! @note Right and Left are when camera will horizontally move laterally
        //! @note Knowing our camera's distance (TODO --- Should consider our local and global distance)
        // glm::vec3 up, down, forward, backwards, right, left;
        float up, down, left, right;
        glm::vec3 position;
        float fov = 45.0f, aspectRatio=1.778f, nearClip=0.1f, farClip=1000.0f;
        float yaw=0.0f, pitch=0.0f;
        //! @note Controlling our zoom speed
        float zoomSpeed;
        float distance; // Computing what our distance is from our current object
        float rotation;

        //! @note Our camera should be able to be adjustable based on our current viewport dimensions
        glm::vec2 viewportSize;
    };

    //! @note struct to act as our transformation matrix
    struct TransformationMatrix{
        glm::mat4 projection;
        glm::mat4 model;
        glm::mat4 view;
    };

    class Camera{
    public:
    private:
        virtual void CameraOnEvent(Event& event) = 0;
        virtual void CameraOnUpdate(Timestep ts) = 0;
    };
};