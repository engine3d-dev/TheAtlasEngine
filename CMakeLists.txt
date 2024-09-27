cmake_minimum_required(VERSION 3.25)
project(engine3d C CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
if(WIN32)
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders REQUIRED)
endif(WIN32)

if(LINUX)
find_package(VulkanLoader REQUIRED)
endif(LINUX)

find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
# find_package(imguidocking REQUIRED)
find_package(box2d REQUIRED)
# find_package(joltphysics REQUIRED)
# find_package(assimp REQUIRED)

add_subdirectory(src)
# add_subdirectory(Editor)

# Copy to compile_commands.json for .clangd
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
# else()
#     target_compile_options(${PROJECT_NAME} PUBLIC "-fsanitize=address")
endif(MSVC)

# target_include_directory is setting some private settings for differentiating what internal includes are privates and what should be includes should be exposed to the application-dev
target_include_directories(${PROJECT_NAME} PRIVATE engine3d/ engine3d/Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${JoltPhysics_SOURCE_DIR} ${GLM_INCLUDE_DIR} ./engine3d)

if(WIN32)
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
    # imguidocking::imguidocking
    box2d::box2d
    # Jolt::Jolt
)
endif(WIN32)

if(LINUX)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
    Vulkan::Loader
    glm::glm
    fmt::fmt
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    # imguidocking::imguidocking
    box2d::box2d
    Jolt::Jolt
)
endif(LINUX)


install(TARGETS ${PROJECT_NAME})
