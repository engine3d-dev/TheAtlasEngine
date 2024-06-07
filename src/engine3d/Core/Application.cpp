#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Core/Application.h>
#include <engine3d/Core/UI/UILayer.h>

namespace Engine3D{

    /**
     * @name NOTES -- For UILayer/UIWidgets
     * @note NOTE -- just leaving this here until some more rendering work seems good...
     * @note Note-To-Self -- Pausing doing UI-related implementations
     * @note Going to focus on the renderer until we have a cubemap, and various rendering techniques implemented
     * @note  Before moving onto the UI of the engine
     * @note For now, I am going to pause the progression on the UILayer
     * @note Here are some bugs that exist (once resuming implementing the UI-stuff)
     * @note There are bugs when adding the UILayer as it is commented
     * @note Check UILayer on the bug, though reconsider designing how each layers may correspond to the UI
     * @note Look into redesigning how the UI layer may work and how things'll get rendered to the main viewport
     * 
     * @note Here are logs of ideas that I needed to look into for the UI (in cases I forget)
     * @note IDEA-NOTE --- Checking for mouse hovering/focused viewport
     * @note IDEA-NOTE --- Making sures textures are able to get loaded via framebuffer
     * @note IDEA-NOTE --- Making sure that we are able to get drag/drop target and sources working
     * @note IDEA-NOTE --- Disabling the ability to move our main viewport outside our main application window.
     * 
    */
    Application* Application::instance = nullptr;
    static FramebufferMetaData g_FramebufferData;

    Application::Application(const std::string& name) {
        instance = this;
        m_IsAppMinimized = false;
        m_Window = std::unique_ptr<Window>(Window::Create(name));

        //! @note We are setting our defaulted application event to whatever configuration is set within this application state.
        m_Window->SetEventCallback(bind(this, &Application::OnEvent));
        m_IsApplicationRunning = true;
        Pipeline::InitializePipeline(API::OPENGL);

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        // PushOverlay(new UILayer());

        g_FramebufferData.attachments = {TextureFormat::RGBA8, TextureFormat::RED_INTEGER, TextureFormat::DEPTH};
        g_FramebufferData.width = Get().WindowSize().x;
        g_FramebufferData.height = Get().WindowSize().y;
        // g_FramebufferData.width = instance->m_Window->GetWidth();
        // g_FramebufferData.height = instance->m_Widno->GetHeight();


        m_Framebuffer = Framebuffer::Create(g_FramebufferData);
    }

    Application::~Application(){}

    void Application::OnEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event){
            m_IsApplicationRunning = false;
            return false;
        });

        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event){
            if(event.GetWidth() == 0 || event.GetHeight() == 0){
                m_IsAppMinimized = true;
                return false;
            }

            m_IsAppMinimized = false;
            Pipeline::SetViewport(event.GetWidth(), event.GetHeight());
            return false;
        });

        // dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event){
        //     CoreLogWarn("Resize ({} x {})", event.GetWidth(), event.GetHeight());
        //     return false;
        // });

        auto iter = m_LayerStack.end();

        while(iter != m_LayerStack.begin()){
            if(event.IsEventHandled()) break;

            (*--iter)->OnEvent(event);
        }
    }

    void Application::Run(){
        while(m_IsApplicationRunning){
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if(!m_IsAppMinimized){
                //! @note Refreshes our frame with set pipeline properties params set
                //! @note Refresh each frame in our main loop on our window
                m_Framebuffer->ApplyResources();
                Pipeline::RefreshFrame();
                m_Framebuffer->ClearAttachements(1, -1);

                for(auto layer : m_LayerStack){
                    layer->OnUpdate(timestep);
                }

                // m_ImGuiLayer->Begin();
                // for(auto layer : m_LayerStack){
                //     layer->OnUIRender();
                // }
                // m_ImGuiLayer->End();
                m_ImGuiLayer->UpdateUI([this](){
                    for(auto layer : m_LayerStack){
                        layer->OnUIRender();
                    }
                });
            }

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer){
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer){
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::SetName(const std::string& title){
        instance->m_Window->SetName(title);
    }

    Application& Application::Get(){
        return *instance;
    }

    void Application::Close(){
        instance->CloseApplication();
    }
    
    void Application::CloseApplication() {
        m_IsApplicationRunning = false;
    }

    glm::vec2 Application::WindowSize(){
        return {instance->m_Window->GetWidth(), instance->m_Window->GetHeight()};
    }

    void* Application::GetNativeWindow() { return instance->m_Window->GetNativeWindow(); }

    const FramebufferMetaData& Application::GetFramebufferData(){
        return g_FramebufferData;
    }

    void Application::FramebufferResize(uint32_t w, uint32_t h){
        if(w == 0 || h == 0 || w > Framebuffer::MaxFramebufferSize() || h > Framebuffer::MaxFramebufferSize()){
            CoreLogWarn("Invalid framebuffer size to be resized!");
            assert(false);
        }

        g_FramebufferData.width = w;
        g_FramebufferData.height = h;
        m_Framebuffer = Framebuffer::Create(g_FramebufferData);
    }

    void Application::SetApplicationBlockedEvents(bool blocked){
        instance->m_ImGuiLayer->SetBlockedEvents(blocked);
    }

    const Framebuffer& Application::CurrentApplicationFramebuffer(){
        return *instance->m_Framebuffer;
    }
};