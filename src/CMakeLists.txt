set(INCLUDE_DIR ../engine3d)
set(SRC_DIR engine3d)

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
    ${INCLUDE_DIR}/core/event/input_poll.hpp
    ${INCLUDE_DIR}/core/event/key_event.hpp
    ${INCLUDE_DIR}/core/event/mouse_event.hpp

    ${INCLUDE_DIR}/thread_utils/thread_utils.hpp
    # ====================================================================================
    # ====================================================================================
    # ====================================================================================
    ${INCLUDE_DIR}/core/engine_logger.hpp
    ${INCLUDE_DIR}/core/window.hpp
    ${INCLUDE_DIR}/core/update_handlers/thread_manager.hpp
    ${INCLUDE_DIR}/core/update_handlers/global_update_manager.hpp
    ${INCLUDE_DIR}/core/update_handlers/sync_update_manager.hpp

    ## Renderer stuff
    ${INCLUDE_DIR}/renderer/renderer.hpp
    ${INCLUDE_DIR}/drivers/renderer_backend.hpp

    ## ui-stuff backend
    ${INCLUDE_DIR}/drivers/ui/imgui_backend.hpp

    ## graphic-agnostic drivers
    ${INCLUDE_DIR}/drivers/swapchain.hpp

    ## Vulkan Driver stuff
    ${VULKAN_INCLUDE_DIR}/vulkan_window.hpp
    ${VULKAN_INCLUDE_DIR}/helper_functions.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_context.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_physical_driver.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_driver.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_swapchain.hpp
    ${VULKAN_INCLUDE_DIR}/vulkan_renderer.hpp

)

set(
    all_src
    ${SRC_DIR}/core/application_instance.cpp
    ${SRC_DIR}/core/event/input_poll.cpp

    ${SRC_DIR}/thread_utils/thread_utils.cpp

    ${SRC_DIR}/core/engine_logger.cpp
    ${SRC_DIR}/core/window.cpp
    ${SRC_DIR}/core/update_handlers/thread_manager.cpp
    ${SRC_DIR}/core/update_handlers/global_update_manager.cpp
    ${SRC_DIR}/core/update_handlers/sync_update_manager.cpp
    ${SRC_DIR}/core/update_handlers/timer.cpp

    ${SRC_DIR}/core/platforms/main.cpp
    ${SRC_DIR}/core/platforms/win32.cpp

    # Renderer-stuff
    ${SRC_DIR}/renderer/renderer.cpp
    ${SRC_DIR}/drivers/renderer_backend.cpp

    ## ui-stuff backend
    ${SRC_DIR}/drivers/ui/imgui_backend.cpp

    ## graphic drivers backend-agnostic
    ${SRC_DIR}/drivers/swapchain.cpp

    ## Vulkan Driver stuff
    ${VULKAN_SRC_DIR}/vulkan_window.cpp
    ${VULKAN_SRC_DIR}/helper_functions.cpp
    ${VULKAN_SRC_DIR}/vulkan_context.cpp
    ${VULKAN_SRC_DIR}/vulkan_physical_driver.cpp
    ${VULKAN_SRC_DIR}/vulkan_driver.cpp
    ${VULKAN_SRC_DIR}/vulkan_swapchain.cpp
    ${VULKAN_SRC_DIR}/vulkan_renderer.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})