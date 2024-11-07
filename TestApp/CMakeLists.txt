cmake_minimum_required(VERSION 3.15)
project(TestApp CXX)


# find_package(Vulkan REQUIRED)

# find_package(engine3d CONFIG REQUIRED)
add_executable(${PROJECT_NAME}
    Application.cpp 

    #To many components
    SceneTest/Scenes/Assets/Components/testComp.hpp 
    SceneTest/src/Scenes/Assets/Components/testComp.cpp
    SceneTest/Scenes/Assets/Components/Transform.hpp
    SceneTest/src/Scenes/Assets/Components/Transform.cpp

    #Scenes
    SceneTest/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp
    SceneTest/src/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.cpp
    
    )

target_include_directories(${PROJECT_NAME} PUBLIC SceneTest)
target_include_directories(${PROJECT_NAME} PRIVATE ../)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    fmt::fmt
    spdlog::spdlog
    glm::glm
    yaml-cpp::yaml-cpp
    Vulkan::Vulkan
    # imguidocking::imguidocking
    # engine3d::engine3d
    engine3d
)