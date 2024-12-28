cmake_minimum_required(VERSION 3.25)
project(engine3d CXX)

set(CMAKE_CXX_STANDARD 23)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

build_library(
    DIRECTORIES src TestApp #Editor
    
    PACKAGES tinyobjloader
    LINK_PACKAGES
    tinyobjloader::tinyobjloader
)