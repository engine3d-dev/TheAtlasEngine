cmake_minimum_required(VERSION 3.27)
project(Editor CXX)


build_demos(
    SOURCES Application.cpp
    LINK_PACKAGES engine3d
)