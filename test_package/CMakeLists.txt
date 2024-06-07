cmake_minimum_required(VERSION 3.15)
project(example CXX)

find_package(engine3d CONFIG REQUIRED)



add_executable(example src/Application.cpp)

# file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY Resources DESTINATION build PATTERN build EXCLUDE)

target_link_libraries(${PROJECT_NAME} engine3d::engine3d)
