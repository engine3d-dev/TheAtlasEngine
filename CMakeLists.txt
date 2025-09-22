cmake_minimum_required(VERSION 3.27)
project(atlas C CXX)
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

build_core_library(
    DIRECTORIES src

    ENABLE_TESTS ${ENABLE_TESTS_ONLY}

    UNIT_TEST_SOURCES

    tests/main.test.cpp
    tests/basic_add.test.cpp
    tests/entity_component_system.test.cpp
    tests/math.test.cpp
    tests/scene.test.cpp
    tests/jolt_type_conversion.test.cpp
    tests/jolt_engine.test.cpp
    # tests/object.test.cpp

    PACKAGES
    tinyobjloader
    TinyGLTF
    # shaderc # uncomment to use shaderc
    ${SHADERC_PACKAGE}
    watcher
    vulkan-cpp

    LINK_PACKAGES
    tinyobjloader::tinyobjloader
    TinyGLTF::TinyGLTF
    # shaderc::shaderc # uncomment to use shaderc
    ${SHADERC_LINK_PACKAGE}
    watcher::watcher
    vulkan-cpp::vulkan-cpp
)

install(TARGETS ${PROJECT_NAME})
