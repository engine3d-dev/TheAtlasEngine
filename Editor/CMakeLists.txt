cmake_minimum_required(VERSION 3.27)
project(editor CXX)

build_application(
    SOURCES
    application.cpp

    editor_world.hpp
    editor_world.cpp

    level_scene.hpp
    level_scene.cpp
    
    LINK_PACKAGES
    atlas
)