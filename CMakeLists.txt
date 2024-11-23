cmake_minimum_required(VERSION 3.25)
project(engine3d CXX)

set(CMAKE_CXX_STANDARD 23)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

find_package(tinyobjloader REQUIRED)

build_library(
    DIRECTORIES src Editor TestApp
    LINK_LIBRARIES tinyobjloader::tinyobjloader
)