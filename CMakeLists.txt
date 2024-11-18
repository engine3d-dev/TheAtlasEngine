cmake_minimum_required(VERSION 3.25)
project(engine3d CXX)

set(CMAKE_CXX_STANDARD 23)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/engine3d)

cmake_minimum_required(VERSION 3.25)
project(engine3d C CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)

add_subdirectory(Editor)
add_subdirectory(TestApp)
add_subdirectory(src)

# # target_include_directory is setting some private settings for differentiating what internal includes are privates and what should be includes should be exposed to the application-dev
target_include_directories(${PROJECT_NAME} PUBLIC ${JoltPhysics_SOURCE_DIR} ${EnTT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE engine3d/ engine3d/Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIR} ./engine3d)

# #Set Compiler definitions
set(is_msvc_cl $<CXX_COMPILER_ID:MSVC>)
set(dev_definitions
    $<${is_msvc_cl}:JPH_FLOATING_POINT_EXCEPTIONS_ENABLED>
    JPH_PROFILE_ENABLED
    JPH_DEBUG_RENDERER
    JPH_OBJECT_STREAM
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${dev_definitions})
