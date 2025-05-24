set(INCLUDE_DIR ../atlas)
set(SRC_DIR atlas)

set(VULKAN_INCLUDE_DIR ${INCLUDE_DIR}/drivers/vulkan)
set(VULKAN_SRC_DIR ${SRC_DIR}/drivers/vulkan)

set(VULKAN_SHADERS_INCLUDE_DIR ${VULKAN_INCLUDE_DIR}/Shaders)
set(VULKAN_SHADERS_SRC_DIR ${VULKAN_SRC_DIR}/Shaders)

set(
    all_headers

    # Event stuff
    ${INCLUDE_DIR}/core/event/key_codes.hpp
    ${INCLUDE_DIR}/core/event/mouse_codes.hpp
    ${INCLUDE_DIR}/core/event/event.hpp
    
    # hash-stuff
    ${INCLUDE_DIR}/core/utilities/hash.hpp

    # Geometry-stuff
    ${INCLUDE_DIR}/core/geometry/mesh.hpp


    # Image stuff
    ${INCLUDE_DIR}/core/image/stb_image.hpp

    # File System Stuff
    ${INCLUDE_DIR}/core/filesystem/file_dialog.hpp

    # serializer stuff
    ${INCLUDE_DIR}/core/serialize/serializer.hpp

    ${INCLUDE_DIR}/thread_utils/thread_utils.hpp
    # ====================================================================================
    # ====================================================================================
    # ====================================================================================
    ${INCLUDE_DIR}/core/engine_logger.hpp
    ${INCLUDE_DIR}/core/window.hpp
    ${INCLUDE_DIR}/core/update_handlers/thread_manager.hpp
    ${INCLUDE_DIR}/core/update_handlers/global_update.hpp
    ${INCLUDE_DIR}/core/update_handlers/sync_update.hpp

    ${INCLUDE_DIR}/core/timer.hpp

    # Math stuff
    ${INCLUDE_DIR}/core/math/math.hpp
    ${INCLUDE_DIR}/core/math/types.hpp

    # System-related stuff
    ${INCLUDE_DIR}/core/system_framework/system_registry.hpp

    #physics stuff
    ${INCLUDE_DIR}/physics/physics_3d/physics_api.hpp
    ${INCLUDE_DIR}/physics/physics_3d/physics_engine.hpp
    ${INCLUDE_DIR}/physics/physics_3d/physics.hpp
    ${INCLUDE_DIR}/physics/physics_3d/physics_context.hpp

    ${INCLUDE_DIR}/physics/physics_3d/jolt/jolt_api.hpp
    ${INCLUDE_DIR}/physics/physics_3d/jolt/jolt_context.hpp
    ${INCLUDE_DIR}/physics/physics_3d/jolt/jolt_helper.hpp

    ${INCLUDE_DIR}/physics/jolt-cpp/jolt_collision_manager.hpp
    ${INCLUDE_DIR}/physics/jolt-cpp/jolt_collision.hpp
    ${INCLUDE_DIR}/physics/jolt-cpp/jolt_contact_listener.hpp

    # Scene Stuff
    ${INCLUDE_DIR}/core/scene/components.hpp
    ${INCLUDE_DIR}/core/scene/scene.hpp
    ${INCLUDE_DIR}/core/scene/world.hpp

    ${INCLUDE_DIR}/core/scene/scene_object.hpp
    ${INCLUDE_DIR}/core/scene/entity.hpp


    ## Renderer stuff
    ${INCLUDE_DIR}/renderer/renderer.hpp
    ${INCLUDE_DIR}/drivers/renderer_backend.hpp

    ## ui-stuff backend
    ${INCLUDE_DIR}/drivers/ui/imgui_backend.hpp
    ${INCLUDE_DIR}/core/ui/widgets.hpp

    ## graphic-agnostic drivers
    ${INCLUDE_DIR}/drivers/vertex_buffer.hpp
    ${INCLUDE_DIR}/drivers/index_buffer.hpp
    ${INCLUDE_DIR}/drivers/shader.hpp
    ${INCLUDE_DIR}/drivers/swapchain.hpp

    ## Vulkan Driver stuff
    ${VULKAN_INCLUDE_DIR}/vulkan_window.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_core.hpp
    ${VULKAN_INCLUDE_DIR}/helper_functions.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_context.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_physical_driver.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_driver.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_swapchain.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_vertex_buffer.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_index_buffer.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_uniform_buffer.hpp
    ${VULKAN_INCLUDE_DIR}/buffer_tutorial.hpp
    ${VULKAN_INCLUDE_DIR}/descriptor_tutorial.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_uniform_buffer.hpp
    ${VULKAN_INCLUDE_DIR}/shaders/vulkan_shader.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_renderer.hpp

)

set(
    all_src
    ${SRC_DIR}/core/application.cpp
    ${SRC_DIR}/core/event/event.cpp

    ${SRC_DIR}/thread_utils/thread_utils.cpp
    ${SRC_DIR}/core/image/stb_image.cpp

    ${SRC_DIR}/core/engine_logger.cpp
    ${SRC_DIR}/core/window.cpp
    ${SRC_DIR}/core/update_handlers/thread_manager.cpp
    ${SRC_DIR}/core/update_handlers/global_update.cpp
    ${SRC_DIR}/core/update_handlers/sync_update.cpp
    ${SRC_DIR}/core/update_handlers/timer.cpp
    ${SRC_DIR}/core/timer.cpp

    ${SRC_DIR}/core/platforms/main.cpp
    ${SRC_DIR}/core/platforms/win32.cpp

    # System-related stuff
    ${SRC_DIR}/core/system_framework/system_registry.cpp

    # Filesystem stuff
    ${SRC_DIR}/core/filesystem/file_dialog.cpp

    # serializer stuff
    ${SRC_DIR}/core/serialize/serializer.cpp

    # jolt
    ${SRC_DIR}/physics/physics_3d/jolt/jolt_api.cpp
    ${SRC_DIR}/physics/physics_3d/jolt/jolt_context.cpp
    ${SRC_DIR}/physics/physics_3d/jolt/jolt_helper.cpp

    # physics stuff
    ${SRC_DIR}/physics/physics_3d/physics_engine.cpp
    ${SRC_DIR}/physics/physics_3d/physics_api.cpp
    ${SRC_DIR}/physics/physics_3d/physics_context.cpp
    ${SRC_DIR}/physics/physics_3d/physics.cpp

    ${SRC_DIR}/physics/jolt-cpp/jolt_collision_manager.cpp
    ${SRC_DIR}/physics/jolt-cpp/jolt_collision.cpp
    ${SRC_DIR}/physics/jolt-cpp/jolt_contact_listener.cpp

    # Scene stuff
    ${SRC_DIR}/core/scene/world.cpp
    ${SRC_DIR}/core/scene/entity.cpp

    # Geometry-stuff
    ${SRC_DIR}/core/geometry/mesh.cpp

    # Renderer-stuff
    ${SRC_DIR}/renderer/renderer.cpp
    ${SRC_DIR}/drivers/renderer_backend.cpp

    ## ui-stuff backend
    ${SRC_DIR}/drivers/ui/imgui_backend.cpp
    
    ${SRC_DIR}/core/ui/widgets.cpp

    ## graphic drivers backend-agnostic
    ${SRC_DIR}/drivers/swapchain.cpp
    ${SRC_DIR}/drivers/vertex_buffer.cpp
    ${SRC_DIR}/drivers/index_buffer.cpp
    ${SRC_DIR}/drivers/shader.cpp

    ## Vulkan Driver stuff
    ${VULKAN_SRC_DIR}/vulkan_window.cpp
    ${VULKAN_SRC_DIR}/vulkan_core.cpp
    ${VULKAN_SRC_DIR}/helper_functions.cpp
    ${VULKAN_SRC_DIR}/vulkan_context.cpp
    ${VULKAN_SRC_DIR}/vulkan_physical_driver.cpp
    ${VULKAN_SRC_DIR}/vulkan_driver.cpp
    ${VULKAN_SRC_DIR}/vulkan_swapchain.cpp
    ${VULKAN_SRC_DIR}/vulkan_vertex_buffer.cpp
    ${VULKAN_SRC_DIR}/vulkan_index_buffer.cpp
    ${VULKAN_SRC_DIR}/buffer_tutorial.cpp
    ${VULKAN_SRC_DIR}/vulkan_uniform_buffer.cpp
    ${VULKAN_SRC_DIR}/descriptor_tutorial.cpp
    ${VULKAN_SRC_DIR}/vulkan_uniform_buffer.cpp
    ${VULKAN_SRC_DIR}/shaders/vulkan_shader.cpp
    ${VULKAN_SRC_DIR}/vulkan_renderer.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})
