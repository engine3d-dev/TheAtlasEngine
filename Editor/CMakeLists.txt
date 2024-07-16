cmake_minimum_required(VERSION 3.15)
project(Editor CXX)

include_directories(
    ../
)

set(
    all_src
    Editor/Editor.cpp
    Editor/EngineLayer.cpp
    Editor/UILayer.cpp
    platforms/main.cpp
    platforms/win32.cpp
)


add_executable(${PROJECT_NAME} ${all_src})

# target_link_libraries(${PROJECT_NAME} engine3d)
target_link_libraries(
    ${PROJECT_NAME}
    engine3d
    glfw
    glm::glm
    fmt::fmt
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    box2d::box2d
    # joltphysics::joltphysics
)