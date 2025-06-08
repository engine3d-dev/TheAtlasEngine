set(INCLUDE_DIR ../atlas)
set(SRC_DIR atlas)

set(VULKAN_INCLUDE_DIR ${INCLUDE_DIR}/drivers/vulkan-cpp)
set(VULKAN_SRC_DIR ${SRC_DIR}/drivers/vulkan-cpp)

set(VULKAN_SHADERS_INCLUDE_DIR ${VULKAN_INCLUDE_DIR}/Shaders)
set(VULKAN_SHADERS_SRC_DIR ${VULKAN_SRC_DIR}/Shaders)

set(
    all_src
    ${SRC_DIR}/core/application.cpp
    ${SRC_DIR}/core/event/event.cpp

    # ${SRC_DIR}/thread_utils/thread_utils.cpp
    # ${SRC_DIR}/core/image/stb_image.cpp

    ${SRC_DIR}/core/engine_logger.cpp
    ${SRC_DIR}/core/window.cpp
    # ${SRC_DIR}/core/update_handlers/thread_manager.cpp
    # ${SRC_DIR}/core/update_handlers/global_update.cpp
    ${SRC_DIR}/core/update_handlers/sync_update.cpp
    # ${SRC_DIR}/core/update_handlers/timer.cpp
    # ${SRC_DIR}/core/timer.cpp

    ${SRC_DIR}/core/platforms/main.cpp
    ${SRC_DIR}/core/platforms/win32.cpp

    # System-related stuff
    ${SRC_DIR}/core/system_framework/system_registry.cpp

    # Filesystem stuff
    ${SRC_DIR}/core/filesystem/file_dialog.cpp

    # serializer stuff
    ${SRC_DIR}/core/serialize/serializer.cpp

    # physics stuff
    # ${SRC_DIR}/physics/physics_3d/physics_engine.cpp
    # ${SRC_DIR}/physics/physics_3d/jolt/jolt_api.cpp
    # ${SRC_DIR}/physics/physics_3d/physics_api.cpp
    # ${SRC_DIR}/physics/physics_3d/jolt/jolt_physics_engine.cpp

    # Scene stuff
    ${SRC_DIR}/core/scene/world.cpp
    ${SRC_DIR}/core/scene/entity.cpp

    # Geometry-stuff
    # ${SRC_DIR}/core/geometry/mesh.cpp

    # Renderer-stuff
    ${SRC_DIR}/renderer/renderer.cpp
    ${SRC_DIR}/drivers/render_context.cpp
    ${SRC_DIR}/drivers/graphics_context.cpp
    ${SRC_DIR}/drivers/image/stb_image.cpp

    ## ui-stuff backend
    # ${SRC_DIR}/drivers/ui/imgui_backend.cpp
    
    ${SRC_DIR}/core/ui/widgets.cpp

    ## Vulkan Driver stuff
    ${VULKAN_SRC_DIR}/vk_window.cpp
    # ${VULKAN_SRC_DIR}/vulkan_core.cpp
    ${VULKAN_SRC_DIR}/helper_functions.cpp
    ${VULKAN_SRC_DIR}/vk_context.cpp
    ${VULKAN_SRC_DIR}/vk_physical_driver.cpp
    ${VULKAN_SRC_DIR}/vk_driver.cpp
    ${VULKAN_SRC_DIR}/vk_command_buffer.cpp
    ${VULKAN_SRC_DIR}/vk_swapchain.cpp
    ${VULKAN_SRC_DIR}/vk_present_queue.cpp
    ${VULKAN_SRC_DIR}/vk_renderpass.cpp
    ${VULKAN_SRC_DIR}/vk_shader_group.cpp
    ${VULKAN_SRC_DIR}/vk_pipeline.cpp
    ${VULKAN_SRC_DIR}/vk_vertex_buffer.cpp
    ${VULKAN_SRC_DIR}/vk_index_buffer.cpp
    ${VULKAN_SRC_DIR}/vk_uniform_buffer.cpp
    ${VULKAN_SRC_DIR}/vk_texture.cpp
    ${VULKAN_SRC_DIR}/vk_descriptor_set.cpp
    ${VULKAN_SRC_DIR}/mesh.cpp
    # ${OLD_VULKAN_SRC_DIR}/vulkan_vertex_buffer.cpp
    # ${OLD_VULKAN_SRC_DIR}/vulkan_index_buffer.cpp
    # ${VULKAN_SRC_DIR}/buffer_tutorial.cpp
    # ${VULKAN_SRC_DIR}/vulkan_uniform_buffer.cpp
    # ${VULKAN_SRC_DIR}/descriptor_tutorial.cpp
    # ${VULKAN_SRC_DIR}/vulkan_uniform_buffer.cpp
    # ${VULKAN_SRC_DIR}/shaders/vulkan_shader.cpp
    ${VULKAN_SRC_DIR}/vk_renderer.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})
