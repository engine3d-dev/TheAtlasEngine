cmake_minimum_required(VERSION 4.0)
project(editor CXX)


add_executable(${PROJECT_NAME} application.cpp)

target_sources(editor PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES
    editor.cppm
    editor_world.cppm
    level_scene.cppm
    level_scene2.cppm
    # content_browser_panel.cppm
    icon.cppm
    utilities.cppm
)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw Vulkan::Vulkan spdlog::spdlog glm::glm atlas)