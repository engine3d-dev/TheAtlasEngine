#pragma once
#include <string>
#include <core/core.hpp>
#include <scene/scene_node.hpp>

namespace engine3d{
    class RendererContext{
    public:
        static Scope<RendererContext> Initialize(const std::string& p_Tag);
        virtual ~RendererContext() = default;

        void Begin();
        void End();

        void RenderSceneNode(Ref<SceneNode> p_SceneContext);
        void RenderSceneObjects(std::map<std::string, Ref<SceneNode>> p_SceneObjects);
        
    private:
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void DrawScene(Ref<SceneNode> p_SceneContext) = 0;
        virtual void DrawSceneObjects(std::map<std::string, Ref<SceneNode>>& p_SceneObjects) = 0;
    };
};