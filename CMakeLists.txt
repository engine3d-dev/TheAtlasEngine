cmake_minimum_required(VERSION 3.15)
project(engine3d C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders REQUIRED)
# find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(box2d REQUIRED)
# find_package(joltphysics REQUIRED)
# For some reason draco is having some errors with ftell in one of it's files
# Draco is a dependency for using assimp
# find_package(assimp REQUIRED)

add_subdirectory(src)
add_subdirectory(Editor)
# add_subdirectory(VulkanRenderer)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIR})

# This will run the build doing -j which is building at full capacity.
# TODO - Should probably have this be disabled by default and have this be a flag set through conan.
# add_custom_target(my_parallel_build
#                       COMMAND ${CMAKE_COMMAND} --build -j
#                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#                       COMMENT "My parallel build with 5 cores")

# Copy to source directory
add_custom_target(
    copy-compile-commands ALL
    DEPENDS
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
    DEPENDS
    # Unlike "proper" targets like executables and libraries, 
    # custom command / target pairs will not set up source
    # file dependencies, so we need to list file explicitly here
    generate-compile-commands
    ${CMAKE_BINARY_DIR}/compile_commands.json
)

# Generate the compilation commands. Necessary so cmake knows where it came
# from and if for some reason you delete it.
add_custom_target(generate-compile-commands
    DEPENDS
        ${CMAKE_BINARY_DIR}/compile_commands.json
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -B${CMAKE_BINARY_DIR} -S${CMAKE_SOURCE_DIR}
)

# This is used because if we do not have this users systems may give them a linked error with oldnames.lib
# Usage - used to suppress that lld-link error and use the defaulted linked .library
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Z1" "/NOD")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif(MSVC)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    glfw
    ${OPENGL_LIBRARIES}
    Vulkan::Vulkan
    vulkan-headers::vulkan-headers
    glm::glm
    fmt::fmt
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    box2d::box2d
    # joltphysics::joltphysics
)

install(TARGETS ${PROJECT_NAME})
