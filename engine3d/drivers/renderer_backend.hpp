#pragma once
#include "scene/scene.hpp"
#include <string>
#include <core/core.hpp>
// #include <scene/scene_object.hpp>

namespace engine3d{
    class SceneObject;
    class RendererContext{
    public:
        static Scope<RendererContext> Initialize(const std::string& p_Tag);
        virtual ~RendererContext() = default;

        void Begin();
        void End();

        void RenderSceneObject(Ref<SceneObject>& p_SceneContext);
        void RenderSceneObjects(const Ref<SceneScope>& p_CurrentSceneCtx);
        void RenderWithCamera(Ref<SceneObject>& p_Object, Ref<SceneObject>& p_CameraObject);
    private:
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void DrawScene(Ref<SceneObject> p_SceneContext) = 0;
        virtual void DrawSceneObjects(const Ref<SceneScope>& p_CurrentSceneCtx) = 0;
        virtual void DrawSceneObject(Ref<SceneObject>& p_CurrentObject) = 0;
        virtual void DrawObjectWithCamera(Ref<SceneObject>& p_Object, Ref<SceneObject>& p_CameraObject) = 0;
    };
};