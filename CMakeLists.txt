cmake_minimum_required(VERSION 3.15)
project(engine3d CXX)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(box2d REQUIRED)
# find_package(joltphysics REQUIRED)
# For some reason draco is having some errors with ftell in one of it's files
# Draco is a dependency for using assimp
# find_package(assimp REQUIRED)

add_subdirectory(src)


target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIR})
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glfw
    glad::glad
    glm::glm
    fmt::fmt
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    box2d::box2d
    # joltphysics::joltphysics
)

install(TARGETS ${PROJECT_NAME})
