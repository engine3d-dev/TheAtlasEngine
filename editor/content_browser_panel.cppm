module;

#include <string>
#include <filesystem>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module content_browser;

import atlas.logger;
import atlas.drivers.vulkan.instance_context;
import atlas.drivers.vulkan.imgui_context;
import vk;

export class content_browser_panel {
public:
    content_browser_panel() : m_current_directory(std::filesystem::path("assets")) {
        console_log_info("content_browser_panel()!");
        vk::texture_info config_texture = {
            .phsyical_memory_properties = atlas::vulkan::instance_context::physical_driver().memory_properties(),
            .filepath = std::filesystem::path("assets/icons/FileIcon.png")
        };
        m_file_icon = vk::texture(atlas::vulkan::instance_context::logical_device(), config_texture);
        if(!m_file_icon.loaded()) {
            console_log_info("Play Button Could not be loaded!!");
        }
        config_texture = {
            .phsyical_memory_properties = atlas::vulkan::instance_context::physical_driver().memory_properties(),
            .filepath = std::filesystem::path("assets/icons/DirectoryIcon.png")
        };
        m_directory_icon = vk::texture(atlas::vulkan::instance_context::logical_device(), config_texture);
        if(!m_directory_icon.loaded()) {
            console_log_info("Stop Button Could not be loaded!!");
        }

        m_file_icon_id = static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(m_file_icon.image().sampler(), m_file_icon.image().image_view(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        m_directory_icon_id = static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(m_directory_icon.image().sampler(), m_directory_icon.image().image_view(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    }


    void run() {
        if(ImGui::Begin("Content Browser")) {
            // console_log_info("Path: {}", m_asset_path.string());
            if(m_current_directory != std::filesystem::path(m_asset_path)){
                if(ImGui::Button("<-")){
                    m_current_directory = m_current_directory.parent_path();
                }
            }

            // Setting up content button properties
            static float padding = 16.0f;
            static float thumbnailSize = 128.0f;
            float cellSize = thumbnailSize + padding;

            float panelWidth = ImGui::GetContentRegionAvail().x;
            int columnCount = (int)(panelWidth / cellSize);

            if(columnCount < 1)
                columnCount = 1;

            ImGui::Columns(columnCount, 0, false);


            // @note First, list all files in directory
            //
            // @note TODO things
            // @note potentially have this be loaded once, by doing something like adding it in a list or std::vector<T>
            // @note then to iterate that list every frame.
            // @note OR could do it per second, to pickup new files (since if file do change)
            
            for(auto& dir_entry : std::filesystem::directory_iterator(m_current_directory)){
                const auto& path = dir_entry.path(); // Absolute Path
                auto relative_path = std::filesystem::relative(path, m_asset_path); 
                std::string filename = relative_path.filename().string();
                
                ImGui::PushID(filename.c_str());
                // Ref<Texture2D> icon = dir_entry.is_directory() ? _directoryIcon : _fileIcon;
                ImTextureID icon = dir_entry.is_directory() ? m_directory_icon_id : m_file_icon_id;
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

                // ImGui::ImageButton(reinterpret_cast<void *>(icon->getRendererID()), {thumbnailSize, thumbnailSize}, { 0, 1 }, { 1, 0});
                ImGui::ImageButton("##Button", icon, ImVec2(thumbnailSize, thumbnailSize), { 1, 0 }, { 0, 1 });
                    
                if(ImGui::BeginDragDropSource()){
                    std::string itemPath = relative_path.string();
                    // @note keep in mind sizeof(itemPath) is in bytes
                    ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath.c_str(), itemPath.size());
                    ImGui::EndDragDropSource();
                }

                ImGui::PopStyleColor();

                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                    console_log_warn("MouseLeftClick in Content Browser Panel!");
                    console_log_info("Absolute Path: {}", path.filename().string());
                    if(dir_entry.is_directory()){
                        m_current_directory /= path.filename();
                        console_log_info("Current Dir: {}", m_current_directory.string());
                    }
                }

                ImGui::TextWrapped("%s", filename.c_str());
                ImGui::NextColumn();

                ImGui::PopID();
            }

            ImGui::Columns(1);
            ImGui::SliderFloat("Thumnail Size", &thumbnailSize, 16, 512);
            ImGui::SliderFloat("Padding", &padding, 0, 32);
            
            ImGui::End();
        }
    }


    void destroy() {
        m_file_icon.destroy();
        m_directory_icon.destroy();
    }

private:
    vk::texture m_file_icon;
    vk::texture m_directory_icon;
    ImTextureID m_file_icon_id;
    ImTextureID m_directory_icon_id;
    const std::filesystem::path m_project_path = std::filesystem::current_path();
    const std::filesystem::path m_asset_path = m_project_path / "assets";
    std::filesystem::path m_current_directory;
};