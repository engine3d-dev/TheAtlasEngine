module;

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

#include <GLFW/glfw3.h>

export module atlas.core.editor:dockspace;

export namespace atlas::ui {
    /**
     * @brief dockspace window using imgui
     */
    class dockspace {
    public:
        dockspace() = default;

        void fullscreen(bool p_fullscreen) {
            m_fullscreen_enabled = p_fullscreen;
        }

        void dockspace_open(bool p_dockspace_open) {
            m_is_dockspace_open = p_dockspace_open;
        }

        bool begin() {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGuiWindowFlags window_flags =
              ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
              ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

            // Always sync dockspace to main viewport so it resizes with the
            // host window
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            // ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

            if (m_fullscreen_enabled) {
                window_flags |= ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                                ImGuiWindowFlags_NoNavFocus;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
                window_flags |= ImGuiWindowFlags_NoBackground;
            }
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                                ImVec2(0.0f, 0.0f));
            m_is_begin = ImGui::Begin(
              "Dockspace Demo", &m_is_dockspace_open, window_flags);
            ImGui::PopStyleVar();

            // Dockspace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
                ImGui::DockSpace(
                  dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
            }

            return m_is_begin;
        }

        void end() { ImGui::End(); }

    private:
        bool m_fullscreen_enabled = false;
        bool m_is_dockspace_open = false;
        bool m_is_begin = false;
    };

};