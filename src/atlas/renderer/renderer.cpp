#include <renderer/renderer.hpp>
#include <drivers/renderer_backend.hpp>

namespace atlas{
    //! @note Only this renderer should have ownership of this renderer context
    //! @note Renderer context is  a graphics-agnostic renderer that provides an API to interact with our agnostic graphcis API's such as Vulkan, DirectX (if we plan to), etc.
    scope<render_context> g_RendererBackend = nullptr;

    void renderer::initialize(){
        g_RendererBackend = render_context::initialize("Renderer");
    }

    // void renderer::SetCustomShaders(const std::string& p_VertexShader, const std::string& p_FragmentShader, bool p_IsOverwritten){
    // }

    void renderer::begin(){
        return g_RendererBackend->begin();
    }

    void renderer::end(){
        return g_RendererBackend->end();
    }

    uint32_t renderer::get_current_frame(){
        return render_context::get_current_frame();
    }

    // void renderer::set_initial_camera(PerspectiveCamera& camera){
    //     g_RendererBackend->SetCamera(camera);
    // }

    void renderer::set_camera(ref<scene_object>& object){
        g_RendererBackend->set_camera_object(object);
    }

    void renderer::submit_scene_object(ref<scene_object>& p_Object){
        g_RendererBackend->submit_render_scene_object(p_Object);
    }

    void renderer::render_scene_objects(const ref<scene_scope>& p_CurrentSceneContext){
        return g_RendererBackend->render_scene_objects(p_CurrentSceneContext);
    }

    void renderer::render_scene_object(ref<scene_object>& p_ObjectToRender){
        g_RendererBackend->render_scene_object(p_ObjectToRender);
    }

    void renderer::render_with_camera(ref<scene_object>& p_Object, ref<scene_object>& p_CameraObject){
        g_RendererBackend->render_with_camera(p_Object, p_CameraObject);
    }
};