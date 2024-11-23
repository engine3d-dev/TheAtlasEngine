cmake_minimum_required(VERSION 3.25)
project(TestApp CXX)

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

    #Graphics
    Scenes/Assets/Components/Graphics/SpriteRender3D.hpp
    src/Scenes/Assets/Components/Graphics/SpriteRender3D.cpp

    #Graphics Meshes ChildrenMeshes
    Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp
    src/Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.cpp
    Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp
    # src/Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.cpp
    src/Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.cpp
    Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp
    src/Scenes/Assets/Components/Graphics/Meshes/MeshContainer.cpp
    

    LINK_LIBRARIES engine3d
)