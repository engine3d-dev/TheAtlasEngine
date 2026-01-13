cmake_minimum_required(VERSION 3.27)
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

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw Vulkan::Vulkan spdlog::spdlog glm::glm atlas)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES
    editor_world.cppm
    level_scene.cppm
)