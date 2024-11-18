cmake_minimum_required(VERSION 3.15)
project(Editor CXX)

# build_demos(
#     SOURCES
#     Editor/Editor.hpp
#     Editor/Editor.cpp
# )

set(
    all_src
    Editor/Editor.hpp
    Editor/Editor.cpp
)

add_executable(${PROJECT_NAME} ${all_src})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

find_package(Vulkan REQUIRED)
find_package(VulkanHeaders REQUIRED)

if(LINUX)
find_package(VulkanLoader REQUIRED)
endif(LINUX)

# target_include_directories(${PROJECT_NAME} PUBLIC ${ENGINE_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ../)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(imguidocking REQUIRED)
find_package(box2d REQUIRED)
find_package(joltphysics REQUIRED)
find_package(EnTT REQUIRED)


target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
    Vulkan::Vulkan
    vulkan-headers::vulkan-headers
    glm::glm
    fmt::fmt
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    imguidocking::imguidocking
    box2d::box2d
    Jolt::Jolt
    EnTT::EnTT
    engine3d
)

