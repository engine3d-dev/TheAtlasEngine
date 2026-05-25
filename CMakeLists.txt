cmake_minimum_required(VERSION 4.0)

# Generate compile commands for anyone using our libraries.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

project(atlas LANGUAGES CXX)


# TODO: Remove this once shaderc in the github CI is fixed
option(USE_SHADERC "[${PROJECT_NAME}] Enabling shaderc" OFF)
option(ENABLE_TESTS_ONLY "[${PROJECT_NAME}] Enabling unit testing" OFF)

# USE_SHADERC is set in the conanfile.py with `-o enable_shaderc=True`
# CI fails when trying to compile with 
if (USE_SHADERC)
    add_compile_definitions(USE_SHADERC)
    set(SHADERC_PACKAGE shaderc)
    set(SHADERC_LINK_PACKAGE shaderc::shaderc)
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} USE_SHADERC Enabled")
else()
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} USE_SHADERC Disabled")
endif()

# ENABLE_TESTS_ONLY is set in the conanfile.py with `-o enable_tests_only=True`
if(ENABLE_TESTS_ONLY)
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ENABLE_TESTS_ONLY Enabled")
    add_compile_definitions(ENABLE_TESTS_ONLY)
else()
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ENABLE_TESTS_ONLY Disabled")
endif()

# The variable CLANG_FORMAT_PATH is now defined automatically by conan_toolchain.cmake
if(EXISTS "${CLANG_FORMAT_PATH}")
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ${CLANG_FORMAT_PATH} found!")
else()
    message(WARNING "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ${CLANG_FORMAT_PATH} not found!")
endif()

if(EXISTS "${CLANG_TIDY_PATH}")
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ${CLANG_TIDY_PATH} found!")
else()
    message(WARNING "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ${CLANG_TIDY_PATH} not found!")
endif()

if(EXISTS "${CMAKE_PATH}")
    message(STATUS "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ${CMAKE_PATH} found!")
else()
    message(WARNING "${BoldBlue}[${PROJECT_NAME}]:${ColorReset} ${CMAKE_PATH} not found!")
endif()

add_library(${PROJECT_NAME} STATIC)
# add_subdirectory(editor)

if(${ENABLE_TESTS_ONLY})
    add_executable(
        unit_test
        tests/main.test.cpp
    )

    find_package(ut REQUIRED)
    target_link_libraries(unit_test PRIVATE boost-ext-ut::ut atlas)

    target_sources(unit_test PUBLIC
        FILE_SET CXX_MODULES
        TYPE CXX_MODULES
        FILES
        tests/tests.cppm
        tests/basic_add.test.cppm
        tests/entity_component_system.test.cppm
        tests/math.test.cppm
        tests/scene.test.cppm
    )

    add_custom_target(run_tests ALL DEPENDS unit_test COMMAND unit_test)

endif()

set_packages(

    PACKAGES
    glfw3
    Vulkan
    glm
    spdlog
    imguidocking
    Jolt
    yaml-cpp
    stb
    flecs

    tinyobjloader
    ${SHADERC_PACKAGE}
    watcher
    vulkan-cpp
    nfd

    LINK_PACKAGES
    glfw
    Vulkan::Vulkan
    glm::glm
    spdlog::spdlog
    imguidocking::imguidocking

    Jolt::Jolt
    yaml-cpp
    stb::stb
    flecs::flecs_static
    tinyobjloader::tinyobjloader
    ${SHADERC_LINK_PACKAGE}
    watcher::watcher
    vulkan-cpp
    nfd::nfd
)

if(APPLE)
target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -g -Wall -Wextra -Wno-missing-field-initializers -Wshadow
)
else()
target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -g -Wall -Wextra -Wno-missing-field-initializers -Wshadow -msse4.1 -mavx
)
endif()

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES

    # core
    # atlas/atlas.cppm
    atlas/core/api.cppm
    atlas/core/common.cppm
    atlas/core/utilities/types.cppm
    atlas/core/utilities/utilities.cppm
    atlas/core/core.cppm
    atlas/core/logger.cppm
    atlas/core/application.cppm
    atlas/core/window.cppm

    # math
    atlas/core/math/types.cppm
    atlas/core/math/math.cppm

    # scenes
    atlas/core/scene/components.cppm
    atlas/core/scene/game_object.cppm
    atlas/core/scene/scene.cppm
    atlas/core/scene/world.cppm
    atlas/core/scene/level_streamer.cppm
    atlas/core/scene/uuid.cppm

    atlas/core/utilities/state.cppm
    atlas/core/utilities/poll_state.cppm

    # event
    atlas/core/event/event.cppm
    atlas/core/event/bus.cppm
    atlas/core/event/listener.cppm
    atlas/core/event/keys.cppm
    atlas/core/event/mouse_codes.cppm
    atlas/core/event/joystick_codes.cppm
    atlas/core/event/types.cppm

    # editor
    atlas/core/editor/dockspace.cppm
    atlas/core/editor/menu_item.cppm
    atlas/core/ui/widgets.cppm
    atlas/core/filesystem/file_dialog.cppm

    # serialize
    atlas/core/serialize/types.cppm
    atlas/core/serialize/serializer.cppm

    # renderer
    atlas/renderer/context_loader.cppm
    atlas/renderer/renderer.cppm
    
    atlas/drivers/drivers.cppm
    atlas/drivers/graphics_context.cppm

    atlas/drivers/renderer_system.cppm


    atlas/drivers/vulkan/hash.cppm
    atlas/drivers/vulkan/mesh.cppm
    atlas/drivers/vulkan/shader_resource_group.cppm
    atlas/drivers/vulkan/render_system.cppm
    atlas/drivers/vulkan/uniforms.cppm

    # imgui/UI
    atlas/drivers/vulkan/imgui_context.cppm
    atlas/core/ui/imgui_stdlib.cppm

    # physics
    atlas/drivers/physics_context.cppm
    atlas/physics/types.cppm
    atlas/physics/physics_engine.cppm
    
    # jolt_cpp
    atlas/drivers/jolt_cpp/math.cppm
    atlas/drivers/jolt_cpp/broad_phase.cppm
    atlas/drivers/jolt_cpp/contact_listener.cppm
    atlas/drivers/jolt_cpp/context.cppm

    # drivers
    atlas/drivers/vulkan/utilities.cppm
    atlas/drivers/vulkan/instance_context.cppm
    atlas/drivers/vulkan/window_context.cppm
    atlas/drivers/vulkan/physical_device.cppm
    atlas/drivers/vulkan/device.cppm
    atlas/drivers/vulkan/swapchain.cppm
    atlas/drivers/vulkan/environment_map.cppm
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    PRIVATE atlas/core/entry_point/main.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT atlas_targets
    FILE_SET CXX_MODULES DESTINATION "."
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    CXX_MODULES_BMI DESTINATION "bmi"
)

install(
    EXPORT atlas_targets
    FILE "atlas-config.cmake"
    DESTINATION "lib/cmake"
    CXX_MODULES_DIRECTORY "cxx-modules"
)
