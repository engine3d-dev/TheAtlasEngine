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
    # imguidocking
    # Jolt
    # yaml-cpp
    # stb
    # flecs

    # tinyobjloader
    # ${SHADERC_PACKAGE}
    # watcher
    # vulkan-cpp
    # nfd

    LINK_PACKAGES
    glfw
    Vulkan::Vulkan
    glm::glm
    spdlog::spdlog
    # imguidocking::imguidocking

    # Jolt::Jolt
    # yaml-cpp
    # stb::stb
    # flecs::flecs_static
    # tinyobjloader::tinyobjloader
    # ${SHADERC_LINK_PACKAGE}
    # watcher::watcher
    # vulkan-cpp
    # nfd::nfd
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES
    atlas-modules/atlas.cppm
    atlas-modules/core/api.cppm
    atlas-modules/core/common.cppm
    atlas-modules/core/core.cppm
    atlas-modules/core/application.cppm
    atlas-modules/core/window.cppm
    atlas-modules/core/logger.cppm
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
