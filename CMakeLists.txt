cmake_minimum_required(VERSION 3.25)
project(engine3d C CXX)

set(CMAKE_CXX_STANDARD 23)


set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_INCLUDE_DIR}/Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${JoltPhysics_SOURCE_DIR} ${GLM_INCLUDE_DIR} ${ENGINE_INCLUDE_DIR})


install(TARGETS ${PROJECT_NAME})
