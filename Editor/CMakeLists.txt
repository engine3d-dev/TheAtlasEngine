cmake_minimum_required(VERSION 3.27)
project(Editor CXX)


build_demos(
    SOURCES
    Application.cpp

    EditorWorld.hpp
    EditorWorld.cpp

    LevelScene.hpp
    LevelScene.cpp
    
    LINK_PACKAGES
    engine3d
)