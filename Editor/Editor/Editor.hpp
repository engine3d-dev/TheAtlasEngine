#pragma once
#include "Core/internal/Vulkan2Showcase/VulkanModel.hpp"
#include <Core/ApplicationInstance.hpp>
#include <Core/Scene/SceneObject.hpp>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>

namespace engine3d{
    
    /*
    class GravityPhysicsSystem{
    public:
        GravityPhysicsSystem() = default;
        GravityPhysicsSystem(float strength) : m_Strength(strength){}

        void update(std::vector<SceneObject>& p_Objects, unsigned int substeps = 1){
            const float step_data = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime / substeps;

            for(int i = 0; i < substeps; i++){
                step_simulation(p_Objects, step_data);
            }
        }

        glm::vec2 ComputeForce(SceneObject& p_FromObject, SceneObject& p_ToObject) const{
            auto offset = p_FromObject.GetTransform().Translation - p_ToObject.GetTransform().Translation;
            float dist_squared = glm::dot(offset, offset);

            if(glm::abs(dist_squared) < 1e-10f){
                return {.0f, .0f};
            }

            float force = m_Strength * p_ToObject.RigidBodyComponent.mass * p_FromObject.RigidBodyComponent.mass / dist_squared;
            return force * offset / glm::sqrt(dist_squared);
        }

    private:
        void step_simulation(std::vector<SceneObject>& physicsObjs, float dt){

            // Loops through all pairs of objects and applies attractive force between them
            for (auto iterA = physicsObjs.begin(); iterA != physicsObjs.end(); ++iterA) {
                auto& objA = *iterA;
                for (auto iterB = iterA; iterB != physicsObjs.end(); ++iterB) {
                    if (iterA == iterB) continue;
                    auto& objB = *iterB;
            
                    auto force = ComputeForce(objA, objB);
                    objA.RigidBodyComponent.velocity += dt * -force / objA.RigidBodyComponent.mass;
                    objB.RigidBodyComponent.velocity += dt * force / objB.RigidBodyComponent.mass;
                }
            }
        
            // update each objects position based on its final velocity
            for (auto& obj : physicsObjs) {
                obj.GetTransform().Translation += dt * obj.RigidBodyComponent.velocity;
            }
        }

    private:
        float m_Strength;
    };

    class Vec2FieldSystem{
    public:
        Vec2FieldSystem() = default;
        
        void update(const GravityPhysicsSystem& physicsSystem, std::vector<SceneObject>& physicsObjs, std::vector<SceneObject>& vectorField){
            // For each field line we caluclate the net graviation force for that point in space
            for (auto& vf : vectorField) {
                glm::vec2 direction{};

                for (auto& obj : physicsObjs) {
                    direction += physicsSystem.ComputeForce(obj, vf);
                }
            
                // This scales the length of the field line based on the log of the length
                // values were chosen just through trial and error based on what i liked the look
                // of and then the field line is rotated to point in the direction of the field
                vf.GetTransform().scale.x = 0.005f + 0.045f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
                vf.GetTransform().rotation = atan2(direction.y, direction.x);
            }
        }
    };
    */

    //! @note Our actual editor application
    //! @note TODO -- Having a cenrtal application that will be used across both editor and the game(being developed)
    //! @note So when they're packaged, they can be shipped under one application excluding editor stuff and only things relevant to the game.
    class EditorApplication : public ApplicationInstance{
    public:
        EditorApplication(const std::string& debugName = "Engine3D Editor");
        virtual ~EditorApplication();
        
        //! @note TODO -- Probably have a cleanup handler for this
        //! @note Such as WorldCleanup() or some API to make sure we can cleanly deallocate and delete things...
        // void ShutdownEditor();
        // void UpdateThisApplicationInstance() override;
        void OnApplicationUpdate();
        
    private:
        Ref<vk::VulkanModel> m_SquareMesh;
        Ref<vk::VulkanModel> m_CircleMesh;

        std::vector<SceneObject> m_SceneGameObjects;
        std::vector<SceneObject> m_VectorFields;

        // GravityPhysicsSystem m_GravitySystem;
        // Vec2FieldSystem m_VectorFieldSystem;
        Ref<vk::VulkanModel> m_CubeMesh;



    };
};