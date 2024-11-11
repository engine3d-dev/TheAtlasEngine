cmake_minimum_required(VERSION 3.15)
project(TestApp CXX)


# find_package(Vulkan REQUIRED)

# find_package(engine3d CONFIG REQUIRED)
add_executable(${PROJECT_NAME}
    Application.cpp

    #To Test
    SceneTest/Scenes/Assets/Components/testComp.hpp 
    SceneTest/src/Scenes/Assets/Components/testComp.cpp

    #Transform
    SceneTest/Scenes/Assets/Components/Transform.hpp
    SceneTest/src/Scenes/Assets/Components/Transform.cpp

    #Physics
    SceneTest/Scenes/Assets/Components/PhysicsBody3D.hpp
    SceneTest/src/Scenes/Assets/Components/PhysicsBody3D.cpp

    #Scenes
    SceneTest/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp
    SceneTest/src/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.cpp

    
    )

target_include_directories(${PROJECT_NAME} PUBLIC SceneTest)
target_include_directories(${PROJECT_NAME} PRIVATE ../)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${dev_definitions})

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Vulkan REQUIRED)
find_package(joltphysics REQUIRED)
find_package(EnTT REQUIRED)


target_link_libraries(
    ${PROJECT_NAME}
    fmt::fmt
    spdlog::spdlog
    glm::glm
    yaml-cpp::yaml-cpp
    Vulkan::Vulkan
    Jolt::Jolt
    EnTT::EnTT
    imguidocking::imguidocking
    engine3d
)