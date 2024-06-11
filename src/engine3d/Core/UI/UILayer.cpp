#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Core/UI/UILayer.h>
#include <engine3d/Core/UI/UIWidgets.h>
#include <engine3d/Graphics/Framebuffer.h>

namespace engine3d{
    static glm::vec2 g_ViewportSizeBounds[1];
    static glm::vec2 g_ViewportSize;

    void UILayer::Attach(){
        // m_IsViewportHovered = false;
        // m_IsViewportFocused = false;
    }

    void UILayer::Detach(){}

    void UILayer::UpdateLayer(Timestep ts){
        auto& currentApplication = Application::Get();
        if(FramebufferMetaData spec = Application::GetFramebufferData();
				g_ViewportSize.x > 0.0f and g_ViewportSize.y > 0.0f &&
				(spec.width != g_ViewportSize.x || spec.height != g_ViewportSize.y)){
			// framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            currentApplication.FramebufferResize((uint32_t)g_ViewportSize.x, (uint32_t)g_ViewportSize.y);
			// editorCamera.setViewportSize(g_ViewportSize.x, g_ViewportSize.y);
			// currentlyActiveScene->onViewportResize(g_ViewportSize.x, g_ViewportSize.y); // viewport resizing every time the window size is changed
		}
    }

    void UILayer::UpdateEvent(Event& event){}

    void UILayer::UIRender(){
        auto& currentApplication = Application::Get();

        static bool dockSpaceOpened = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen){
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpened, window_flags);
		ImGui::PopStyleVar();

		if(opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		float minWinSizeX = 370.0f;
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 370.0f;
		

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

        UI::UI_RenderViewport("Viewport", [this, &currentApplication](){
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            m_IsViewportFocused = ImGui::IsWindowFocused();
            m_IsViewportHovered = ImGui::IsWindowHovered();

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();

            g_ViewportSizeBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
            g_ViewportSizeBounds[0] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

            Application::SetApplicationBlockedEvents(!m_IsViewportFocused && !m_IsViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            g_ViewportSize = {g_ViewportSize.x, g_ViewportSize.y};

            if(g_ViewportSize != *((glm::vec2 *)&viewportPanelSize) and viewportPanelSize.x > 0 and viewportPanelSize.y > 0){
                currentApplication.FramebufferResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                g_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
            }

            // auto& currentFramebuffer = Application::CurrentApplicationFramebuffer();
            // ImGui::Image(reinterpret_cast<void *>(currentFramebuffer.GetAttachmentID()), ImVec2(g_ViewportSize.x, g_ViewportSize.y), ImVec2(0, 0), ImVec2(1, 0));


            ImGui::PopStyleVar();


        });

        ImGui::End();
    }
};