cmake_minimum_required(VERSION 3.15)
project(example CXX)

find_package(Vulkan REQUIRED)
find_package(imguidocking REQUIRED)
find_package(engine3d CONFIG REQUIRED)



add_executable(example Application.h Application.cpp EngineLayer.h EngineLayer.cpp UILayer.h UILayer.cpp)

# set_target_properties( ${PROJECT_NAME}
#     PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../"
# )

target_link_libraries(${PROJECT_NAME}
    Vulkan::Vulkan
    imguidocking::imguidocking
    engine3d::engine3d
)