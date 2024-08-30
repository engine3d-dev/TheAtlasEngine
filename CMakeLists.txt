cmake_minimum_required(VERSION 3.25)
project(engine3d C CXX)

set(CMAKE_CXX_STANDARD 23)

# Enabling fsanitize=address on all platforms

# if(WIN32)
#     set(CMAKE_CXX_FLAGS "-static /fsanitize=address")
# else()
#     set(CMAKE_CXX_FLAGS "-fsanitize=address")
# endif()

# 4 is 4, ten is ten, 14 is 14, 40 is 40

# Setting the makefile location in chocolatey
# set(CMAKE_MAKE_PROGRAM  "${env:ChocolateyInstall}")
# set(CMAKE_GENERATOR "Unix Makefiles")
# message("generator is set to ${CMAKE_GENERATOR}/make.exe")
# set(CMAKE_MAKE_PROGRAM $ENV{ChocolateyInstall}/make.exe)
# message("Testing message from engine3d/CMakeLists.txt!!!")
# message("Choco make directory ===============>>> ${CMAKE_MAKE_PROGRAM}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
# find_package(box2d REQUIRED)
find_package(imguidocking REQUIRED)
find_package(joltphysics REQUIRED)
# find_package(shaderc REQUIRED)
# find_package(SPIRV-Tools REQUIRED)

# include(FetchContent)

# Include Jolt (3D Physics framework)
# FetchContent_Declare(
#     JoltPhysics
#     GIT_REPOSITORY "https://github.com/engine3d-dev/JoltPhysics"
#     SOURCE_SUBDIR "build"
# )

# FetchContent_MakeAvailable(JoltPhysics)

# find_package(shaderc REQUIRED)
# find_package(joltphysics REQUIRED)
# For some reason draco is having some errors with ftell in one of it's files
# Draco is a dependency for using assimp
# find_package(assimp REQUIRED)
# find_package(shaderc REQUIRED)

add_subdirectory(src)
# add_subdirectory(Editor)
# add_subdirectory(Testbed)

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
# else()
#     target_compile_options(${PROJECT_NAME} PUBLIC "-fsanitize=address")
endif(MSVC)

# target_include_directory is setting some private settings for differentiating what internal includes are privates and what should be includes should be exposed to the application-dev
target_include_directories(${PROJECT_NAME} PRIVATE engine3d/ engine3d/Core)
# message("PROJECT SOURCE DIR ====>>> ${CMAKE_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC ${JoltPhysics_SOURCE_DIR} ${GLM_INCLUDE_DIR} ./engine3d)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
    Vulkan::Vulkan
    # vulkan-headers::vulkan-headers
    glm::glm
    fmt::fmt
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    # box2d::box2d
    # Jolt
    imguidocking::imguidocking
    Jolt::Jolt
    # shaderc::shaderc
)

install(TARGETS ${PROJECT_NAME})
