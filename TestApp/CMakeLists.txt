cmake_minimum_required(VERSION 3.15)
project(TestApp CXX)

# find_package(Vulkan REQUIRED)

# find_package(engine3d CONFIG REQUIRED)
build_demos(
    SOURCES
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

    LINK_LIBRARIES engine3d
)

target_include_directories(${PROJECT_NAME} PUBLIC SceneTest)
target_include_directories(${PROJECT_NAME} PRIVATE ../)

# target_compile_definitions(${PROJECT_NAME} PRIVATE ${dev_definitions})

# find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)

# find_package(Vulkan REQUIRED)
# find_package(VulkanHeaders REQUIRED)

# if(LINUX)
# find_package(VulkanLoader REQUIRED)
# endif(LINUX)

# # target_include_directories(${PROJECT_NAME} PUBLIC ${ENGINE_INCLUDE_DIR})
# target_include_directories(${PROJECT_NAME} PRIVATE ../)
# find_package(glm REQUIRED)
# find_package(fmt REQUIRED)
# find_package(spdlog REQUIRED)
# find_package(yaml-cpp REQUIRED)
# find_package(imguidocking REQUIRED)
# find_package(box2d REQUIRED)
# find_package(joltphysics REQUIRED)
# find_package(EnTT REQUIRED)


# target_link_libraries(
#     ${PROJECT_NAME}
#     PRIVATE
#     glfw
#     ${OPENGL_LIBRARIES}
#     Vulkan::Vulkan
#     vulkan-headers::vulkan-headers
#     glm::glm
#     fmt::fmt
#     spdlog::spdlog
#     yaml-cpp::yaml-cpp
#     imguidocking::imguidocking
#     box2d::box2d
#     Jolt::Jolt
#     EnTT::EnTT
#     engine3d
# )