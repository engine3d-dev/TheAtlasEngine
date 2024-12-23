cmake_minimum_required(VERSION 3.25)
project(engine3d CXX)

set(CMAKE_CXX_STANDARD 23)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

find_package(tinyobjloader REQUIRED)
find_package(engine3d-audio REQUIRED)

build_library(
    DIRECTORIES src Editor
    LINK_LIBRARIES tinyobjloader::tinyobjloader engine3d-audio::engine3d-audio
)