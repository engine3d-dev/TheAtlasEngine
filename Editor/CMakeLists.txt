cmake_minimum_required(VERSION 3.15)
project(Editor CXX)

# build_demos(
#     SOURCES
#     Editor/Editor.hpp
#     Editor/Editor.cpp
# )

build_demos(
    SOURCES
    Editor/Editor.hpp
    Editor/Editor.cpp
    LINK_LIBRARIES engine3d
)
