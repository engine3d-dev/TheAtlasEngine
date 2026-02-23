cmake_minimum_required(VERSION 4.0)
project(editor CXX)

# build_application(
#     SOURCES
#     application.cpp

#     PACKAGES
#     glfw3
#     Vulkan
#     glm
#     spdlog


#     LINK_PACKAGES
#     atlas
#     glfw
#     Vulkan::Vulkan
#     glm::glm
#     spdlog::spdlog
# )

add_executable(${PROJECT_NAME} application.cpp)

target_sources(editor PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES
    editor_world.cppm
    level_scene.cppm
    content_browser_panel.cppm
)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw Vulkan::Vulkan spdlog::spdlog glm::glm atlas)