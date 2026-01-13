cmake_minimum_required(VERSION 4.0)

# Generate compile commands for anyone using our libraries.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(atlas LANGUAGES CXX)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/atlas)

# TODO: Remove this once shaderc in the github CI is fixed
option(USE_SHADERC "[${PROJECT_NAME}] Enabling shaderc" OFF)
option(ENABLE_TESTS_ONLY "[${PROJECT_NAME}] Enabling unit testing" OFF)

# USE_SHADERC is set in the conanfile.py with `-o enable_shaderc=True`
# CI fails when trying to compile with 
if (USE_SHADERC)
    add_compile_definitions(USE_SHADERC)
    set(SHADERC_PACKAGE shaderc)
    set(SHADERC_LINK_PACKAGE shaderc::shaderc)
    message(STATUS "USE_SHADERC macro enabled.")
else()
    message(STATUS "USE_SHADERC macro NOT enabled.")
endif()

# ENABLE_TESTS_ONLY is set in the conanfile.py with `-o enable_tests_only=True`
if(ENABLE_TESTS_ONLY)
    message(STATUS "ENABLE_TESTS_ONLY macro is enabled")
    add_compile_definitions(ENABLE_TESTS_ONLY)
else()
    message(STATUS "ENABLE_TESTS_ONLY macro not enabled")
endif()

# build_core_library(
#     DIRECTORIES src editor

#     ENABLE_TESTS ${ENABLE_TESTS_ONLY}

#     UNIT_TEST_SOURCES

#     tests/main.test.cpp
#     tests/basic_add.test.cpp
#     tests/entity_component_system.test.cpp
#     tests/math.test.cpp
#     tests/scene.test.cpp
#     tests/jolt_type_conversion.test.cpp
#     tests/jolt_engine.test.cpp

#     PACKAGES
#     tinyobjloader
#     ${SHADERC_PACKAGE}
#     watcher
#     vulkan-cpp`
#     nfd

#     LINK_PACKAGES
#     tinyobjloader::tinyobjloader
#     ${SHADERC_LINK_PACKAGE}
#     watcher::watcher
#     vulkan-cpp
#     nfd::nfd
# )

add_library(${PROJECT_NAME} STATIC)
add_subdirectory(editor_modules)

set_packages(
    ENABLE_TESTS OFF

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

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -g -Wall -Wextra -Wno-missing-field-initializers -Wshadow -msse4.1 -mavx
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES
    atlas-modules/atlas.cppm
    atlas-modules/core/api.cppm
    atlas-modules/core/common.cppm
    atlas-modules/core/utilities/types.cppm
    atlas-modules/core/core.cppm
    atlas-modules/core/logger.cppm
    atlas-modules/core/application.cppm
    atlas-modules/core/window.cppm

    # math
    atlas-modules/core/math/types.cppm
    atlas-modules/core/math/math.cppm

    atlas-modules/core/scene/components.cppm
    atlas-modules/core/scene/game_object.cppm
    atlas-modules/core/scene/scene.cppm
    atlas-modules/core/scene/world.cppm
    atlas-modules/core/scene/system_registry.cppm

    atlas-modules/core/utilities/state.cppm
    atlas-modules/core/utilities/poll_state.cppm

    atlas-modules/core/event/event.cppm
    atlas-modules/core/event/bus.cppm
    atlas-modules/core/event/listener.cppm
    atlas-modules/core/event/keys.cppm
    atlas-modules/core/event/mouse_codes.cppm
    atlas-modules/core/event/joystick_codes.cppm
    atlas-modules/core/event/types.cppm

    # editor
    atlas-modules/core/editor/dockspace.cppm
    atlas-modules/core/editor/menu_item.cppm
    atlas-modules/core/ui/widgets.cppm
    atlas-modules/core/filesystem/file_dialog.cppm

    # serialize
    atlas-modules/core/serialize/types.cppm
    atlas-modules/core/serialize/serializer.cppm

    atlas-modules/renderer/renderer.cppm
    
    atlas-modules/drivers/drivers.cppm
    atlas-modules/drivers/graphics_context.cppm

    atlas-modules/drivers/renderer_system.cppm
    atlas-modules/drivers/vulkan/hash.cppm
    atlas-modules/drivers/vulkan/mesh.cppm
    atlas-modules/drivers/vulkan/shader_resource_group.cppm
    atlas-modules/drivers/vulkan/render_system.cppm
    atlas-modules/drivers/vulkan/uniforms.cppm

    atlas-modules/drivers/vulkan/imgui_context.cppm

    # physics
    atlas-modules/drivers/physics_context.cppm
    atlas-modules/physics/types.cppm
    atlas-modules/physics/physics_engine.cppm
    
    # jolt_cpp
    atlas-modules/drivers/jolt_cpp/math.cppm
    atlas-modules/drivers/jolt_cpp/broad_phase.cppm
    atlas-modules/drivers/jolt_cpp/contact_listener.cppm
    atlas-modules/drivers/jolt_cpp/context.cppm


    atlas-modules/drivers/vulkan/vulkan.cppm
    atlas-modules/drivers/vulkan/utilities.cppm
    atlas-modules/drivers/vulkan/instance_context.cppm
    atlas-modules/drivers/vulkan/window_context.cppm
    atlas-modules/drivers/vulkan/physical_device.cppm
    atlas-modules/drivers/vulkan/device.cppm
    atlas-modules/drivers/vulkan/swapchain.cppm

    # entry point
    # atlas-modules/core/entry_point/main.cppm
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    PRIVATE atlas-modules/core/entry_point/main.cpp
)

# target_sources(${PROJECT_NAME} PUBLIC
#     FILE_SET CXX_MODULES
#     TYPE CXX_MODULES
#     PRIVATE atlas-modules/core/main.cpp
# )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# install(TARGETS ${PROJECT_NAME})
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
