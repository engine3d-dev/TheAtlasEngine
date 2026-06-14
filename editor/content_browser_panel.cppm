module;

#include <memory>
#include <string>
#include <filesystem>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module editor:content_browser;

import atlas.core.utilities;
import vk;
import :icon;

export class content_browser_panel {
public:
    content_browser_panel() = default;

    content_browser_panel(std::shared_ptr<vk::device> p_device,
                          uint32_t p_memory_properties) {
        /*NOLINT*/ m_device = p_device;

        m_directory_icon = ui::experimental::icon(
          m_device, p_memory_properties, "assets/icons/DirectoryIcon.png");

        m_file_icon = ui::experimental::icon(
          m_device, p_memory_properties, "assets/icons/FileIcon.png");

        m_back_icon = ui::experimental::icon(
          m_device, p_memory_properties, "assets/icons/Back.png");
    }

    void run() {
        if (ImGui::Begin("Content Browser")) {
            if (m_current_directory != std::filesystem::path(m_asset_path)) {
                // if(ImGui::Button("<-")){
                //     m_current_directory = m_current_directory.parent_path();
                // }
                if (ImGui::ImageButton("##BackButton",
                                       m_back_icon.texture_id(),
                                       ImVec2(10, 10))) {
                    m_current_directory = m_current_directory.parent_path();
                }
            }

            // Setting up content button properties
            static float padding = 16.0f;
            float thumbnail_size = 86.96f; // originally 128.f
            float cell_size = thumbnail_size + padding;

            float panel_width = ImGui::GetContentRegionAvail().x;
            int column_count = static_cast<int>(panel_width / cell_size);

            if (column_count < 1)
                column_count = 1;

            ImGui::Columns(column_count, nullptr, false);

            // @note First, list all files in directory
            //
            // @note TODO things
            // @note potentially have this be loaded once, by doing something
            // like adding it in a list or std::vector<T>
            // @note then to iterate that list every frame.
            // @note OR could do it per second, to pickup new files (since if
            // file do change)

            for (auto& dir_entry :
                 std::filesystem::directory_iterator(m_current_directory)) {
                const auto& path = dir_entry.path(); // Absolute Path
                auto relative_path =
                  std::filesystem::relative(path, m_asset_path);
                std::string filename = relative_path.filename().string();

                ImGui::PushID(filename.c_str());
                VkDescriptorSet icon = dir_entry.is_directory()
                                         ? m_directory_icon.texture_id()
                                         : m_file_icon.texture_id();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

                ImGui::ImageButton("##Button",
                                   icon,
                                   ImVec2(thumbnail_size, thumbnail_size),
                                   { 1, 0 },
                                   { 0, 1 });

                if (ImGui::BeginDragDropSource()) {
                    std::string item_path = relative_path.string();
                    // @note keep in mind sizeof(item_path) is in bytes
                    ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM",
                                              item_path.c_str(),
                                              item_path.size());
                    ImGui::EndDragDropSource();
                }

                ImGui::PopStyleColor();

                if (ImGui::IsItemHovered() &&
                    ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    if (dir_entry.is_directory()) {
                        m_current_directory /= path.filename();
                    }
                }

                ImGui::TextWrapped("%s", filename.c_str());
                ImGui::NextColumn();

                ImGui::PopID();
            }

            ImGui::End();
        }
    }

    void destroy() {
        m_file_icon.destroy();
        m_directory_icon.destroy();
        m_back_icon.destroy();
    }

private:
    std::shared_ptr<vk::device> m_device;
    ui::experimental::icon m_back_icon;
    ui::experimental::icon m_directory_icon;
    ui::experimental::icon m_file_icon;
    std::filesystem::path m_asset_path =
      std::filesystem::current_path() / "assets";
    std::filesystem::path m_current_directory = m_asset_path;
};