cmake_minimum_required(VERSION 3.27)
project(engine3d C CXX)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

build_library(
    DIRECTORIES src Editor
    PACKAGES
    TestApp
    
    LINK_PACKAGES
    
)


install(TARGETS ${PROJECT_NAME})
