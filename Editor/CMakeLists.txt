cmake_minimum_required(VERSION 3.27)
project(Editor CXX)

build_demos(
    SOURCES
    Editor/Editor.hpp
    Editor/Editor.cpp

    Editor/EditorScene.hpp
    Editor/EditorScene.cpp
    
    PACKAGES tinyobjloader
    LINK_PACKAGES
    tinyobjloader::tinyobjloader
    engine3d
)
