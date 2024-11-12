cmake_minimum_required(VERSION 3.15)
project(Editor CXX)

set(
    all_src
    Editor/Editor.hpp
    Editor/Editor.cpp
)

find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Vulkan REQUIRED)
find_package(EnTT REQUIRED)
find_package(joltphysics REQUIRED)
#find_package(imguidocking REQUIRED)
# find_package(engine3d REQUIRED)

add_executable(${PROJECT_NAME} ${all_src})

target_include_directories(${PROJECT_NAME} PRIVATE ../)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${dev_definitions})

target_link_libraries(
    ${PROJECT_NAME}
    spdlog::spdlog
    glm::glm
    yaml-cpp::yaml-cpp
    Vulkan::Vulkan
    EnTT::EnTT
    Jolt::Jolt
   # imguidocking::imguidocking
    # engine3d::engine3d
    engine3d
)