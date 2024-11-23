cmake_minimum_required(VERSION 3.25)
project(TestApp CXX)

set(SCENE_TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/SceneTest)

build_demos(
    SOURCES
    Application.cpp

    #To Test
    Scenes/Assets/Components/testComp.hpp 
    src/Scenes/Assets/Components/testComp.cpp

    #Physics
    Scenes/Assets/Components/Physics/PhysicsBody3D.hpp
    src/Scenes/Assets/Components/Physics/PhysicsBody3D.cpp

    Scenes/Assets/Components/Bodies/BodyContainer.hpp
    src/Scenes/Assets/Components/Bodies/BodyContainer.cpp


    #Body Shapes
    Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp
    src/Scenes/Assets/Components/Bodies/Shapes/BoxShaper.cpp

    Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp
    src/Scenes/Assets/Components/Bodies/Shapes/SphereShaper.cpp

    #Scenes
    Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp
    src/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.cpp

    LINK_LIBRARIES engine3d
)

# target_include_directories(${PROJECT_NAME} PUBLIC ${SCENE_TEST_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ./ ../)
