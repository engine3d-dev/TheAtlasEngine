set(INCLUDE_DIR ../atlas)
set(SRC_DIR atlas)

set(VULKAN_INCLUDE_DIR ${INCLUDE_DIR}/drivers/vulkan-cpp)
set(VULKAN_SRC_DIR ${SRC_DIR}/drivers/vulkan-cpp)

set(
    all_src
    ${SRC_DIR}/core/application.cpp
    ${SRC_DIR}/core/event/event.cpp

    ${SRC_DIR}/core/image/stb_image.cpp

    ${SRC_DIR}/core/engine_logger.cpp
    ${SRC_DIR}/core/window.cpp
    ${SRC_DIR}/core/utilities/state.cpp

    ${SRC_DIR}/core/platforms/main.cpp
    ${SRC_DIR}/core/platforms/win32.cpp

    # System-related stuff
    ${SRC_DIR}/core/scene/scene_object.cpp
    ${SRC_DIR}/core/system/registry.cpp

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
    ${SRC_DIR}/physics/jolt-cpp/jolt_error_handler.cpp

    # Scene stuff
    ${SRC_DIR}/core/scene/world.cpp

    # Renderer-stuff
    ${SRC_DIR}/renderer/renderer.cpp
    ${SRC_DIR}/drivers/render_context.cpp
    ${SRC_DIR}/drivers/graphics_context.cpp

    ${SRC_DIR}/core/ui/widgets.cpp

    # imgui stuff
    ${VULKAN_SRC_DIR}/vk_imgui.cpp

    ## Vulkan Driver stuff
    ${VULKAN_SRC_DIR}/vk_window.cpp
    ${VULKAN_SRC_DIR}/helper_functions.cpp
    ${VULKAN_SRC_DIR}/utilities/utils.cpp
    ${VULKAN_SRC_DIR}/vk_context.cpp
    ${VULKAN_SRC_DIR}/vk_physical_driver.cpp
    ${VULKAN_SRC_DIR}/vk_driver.cpp
    ${VULKAN_SRC_DIR}/vk_command_buffer.cpp
    ${VULKAN_SRC_DIR}/vk_swapchain.cpp
    ${VULKAN_SRC_DIR}/vk_graphics_queue.cpp
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
    ${VULKAN_SRC_DIR}/vk_renderer.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})
