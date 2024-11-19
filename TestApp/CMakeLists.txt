cmake_minimum_required(VERSION 3.15)
project(TestApp CXX)


# find_package(Vulkan REQUIRED)

# find_package(engine3d CONFIG REQUIRED)
add_executable(${PROJECT_NAME}
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
    
    )

target_include_directories(${PROJECT_NAME} PUBLIC ./ SceneTest)
target_include_directories(${PROJECT_NAME} PRIVATE ../)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${dev_definitions})

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Vulkan REQUIRED)
find_package(joltphysics REQUIRED)
find_package(EnTT REQUIRED)
find_package(imguidocking REQUIRED)


target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
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