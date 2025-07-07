cmake_minimum_required(VERSION 3.27)
project(atlas C CXX)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/atlas)

# TODO: Remove this once shaderc in the github CI is fixed
option(USE_SHADERC "[ENGINE] Enabling unit testing" OFF)
if (USE_SHADERC)
    add_compile_definitions(USE_SHADERC)
    message(STATUS "USE_SHADERC macro enabled.")
else()
    message(STATUS "USE_SHADERC macro NOT enabled.")
endif()

build_core_library(
    DIRECTORIES src

    # This is to enable us to control if we want to run test cases or not
    ENABLE_TESTS ON

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
    tinyobjloader
    TinyGLTF
    # shaderc # uncomment to use shaderc
    watcher

    LINK_PACKAGES
    tinyobjloader::tinyobjloader
    TinyGLTF::TinyGLTF
    # shaderc::shaderc # uncomment to use shaderc
    watcher::watcher
)

install(TARGETS ${PROJECT_NAME})
