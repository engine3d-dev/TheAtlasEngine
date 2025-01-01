cmake_minimum_required(VERSION 3.27)
project(engine3d C CXX)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

build_library(
    DIRECTORIES src Editor

    UNIT_TEST_SOURCES

    tests/basic_add.test.cpp
    tests/math.test.cpp
    tests/entity_component_system.test.cpp
    tests/scene.test.cpp
    tests/object.test.cpp
    tests/main.test.cpp

    PACKAGES
    flecs
    tinyobjloader

    LINK_PACKAGES
    flecs::flecs_static
    tinyobjloader::tinyobjloader
)

install(TARGETS ${PROJECT_NAME})
