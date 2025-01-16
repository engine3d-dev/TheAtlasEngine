#pragma once
#include "scene/scene_object.hpp"
#include <string>
#include <core/engine_logger.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/scene/world.hpp>

namespace engine3d{
    /**
     * @name SceneScope
     * @note Self-contained scope of a given scene
    */
    class SceneScope{
    public:
        SceneScope() : m_EntityHandler(nullptr, "Undefined"), m_Tag("Undefined"){}
        SceneScope(const std::string& p_Tag) : m_Tag(p_Tag){
            ConsoleLogInfo("SceneScope::SceneScope with Tag = {} called!", m_Tag);
            WorldScope world = SystemRegistry::GetWorld();
            std::string world_tag = world.GetTag();
            ConsoleLogTrace("World Tag = {}", world_tag);
        }


        Ref<SceneObject> CreateNewObject(const std::string& Tag){
            // return CreateRef<SceneObject>(SystemRegistry::GetWorld()->GetRegistry(), Tag);
            return CreateRef<SceneObject>(SystemRegistry::GetWorld().GetRegistry(), Tag);
        }

        virtual ~SceneScope() = default;


        void OnUpdate() {
            return Update();
        }

        std::string GetTag() { return m_Tag; }
    private:
        virtual void Update() = 0;

    private:
        EntityObject m_EntityHandler;
        std::string m_Tag = "Undefined Tag";
    };
};