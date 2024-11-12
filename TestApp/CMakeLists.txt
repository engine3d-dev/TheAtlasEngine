cmake_minimum_required(VERSION 3.15)
project(TestApp CXX)


# find_package(Vulkan REQUIRED)

# find_package(engine3d CONFIG REQUIRED)
add_executable(${PROJECT_NAME}
    Application.cpp

    #To Test
    SceneTest/Scenes/Assets/Components/testComp.hpp 
    SceneTest/src/Scenes/Assets/Components/testComp.cpp

    #Physics
    SceneTest/Scenes/Assets/Components/Physics/PhysicsBody3D.hpp
    SceneTest/src/Scenes/Assets/Components/Physics/PhysicsBody3D.cpp

    SceneTest/Scenes/Assets/Components/Bodies/BodyContainer.hpp
    SceneTest/src/Scenes/Assets/Components/Bodies/BodyContainer.cpp


    #Body Shapes
    SceneTest/Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp
    SceneTest/src/Scenes/Assets/Components/Bodies/Shapes/BoxShaper.cpp

    SceneTest/Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp
    SceneTest/src/Scenes/Assets/Components/Bodies/Shapes/SphereShaper.cpp

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
   # imguidocking::imguidocking
    engine3d
)