cmake_minimum_required(VERSION 3.27)
project(engine3d C CXX)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

# set(CMAKE_BUILD_TYPE Debug CACHE STRING "CMake Build Type")
# Define the build type based on Conan settings (optional)
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE ${CONAN_BUILD_TYPE} CACHE STRING "CMAKE build type")
endif()

# TODO: Add BUILD_TYPE parameter to specify different build types for engine3d to handle
# Usage: build_library(BUILD_TYPE Release)
build_library(
    DIRECTORIES src Editor

    UNIT_TEST_SOURCES

    # Another issue that can occur with this
    # Is what if we receive -1073741819?
    # This means that either memory corruption, some invalid memory access is happening within the test cases
    tests/main.test.cpp
    tests/basic_add.test.cpp
    tests/entity_component_system.test.cpp
    tests/math.test.cpp
    tests/scene.test.cpp
    # tests/object.test.cpp

    PACKAGES
    flecs
    tinyobjloader

    LINK_PACKAGES
    flecs::flecs_static
    tinyobjloader::tinyobjloader
)

install(TARGETS ${PROJECT_NAME})
