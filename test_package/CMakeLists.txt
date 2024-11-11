cmake_minimum_required(VERSION 3.15)
project(example CXX)

find_package(Vulkan REQUIRED)

find_package(engine3d CONFIG REQUIRED)
add_executable(example Application.hpp Application.cpp)

target_link_libraries(${PROJECT_NAME}
    Vulkan::Vulkan
    # vulkan-headers::vulkan-headers
    imguidocking::imguidocking
    engine3d::engine3d
)