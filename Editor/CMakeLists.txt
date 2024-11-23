cmake_minimum_required(VERSION 3.15)
project(Editor CXX)

# build_demos(
#     SOURCES
#     Editor/Editor.hpp
#     Editor/Editor.cpp
# )
find_package(tinyobjloader REQUIRED)

build_demos(
    SOURCES
    Editor/Editor.hpp
    Editor/Editor.cpp

    Editor/EditorScene.hpp
    Editor/EditorScene.cpp
    LINK_LIBRARIES tinyobjloader::tinyobjloader engine3d
)
