cmake_minimum_required(VERSION 3.15)
project(example CXX)

find_package(engine3d REQUIRED CONFIG)

add_executable(${PROJECT_NAME} Application.hpp Application.cpp)

target_link_libraries(${PROJECT_NAME} engine3d::engine3d)