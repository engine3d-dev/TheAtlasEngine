set(ENGINE_INCLUDE_NAME ../engine3d)
set(ENGINE_INTERNAL_INCLUDE ${ENGINE_INCLUDE_NAME}/Core/internal)
set(ENGINE_SRC_DIR engine3d)
set(VULKAN_INCLUDE_DIR ${ENGINE_INCLUDE_NAME}/Core/internal/VulkanCpp)
set(VULKAN_SRC_DIR ${ENGINE_SRC_DIR}/Core/internal/VulkanCpp)

set(
    all_headers

    ${ENGINE_INCLUDE_NAME}/Core/ApplicationInstance.hpp

    ${ENGINE_INCLUDE_NAME}/Core/EngineLogger.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Window.hpp

    # Event system includes
    ${ENGINE_INCLUDE_NAME}/Core/Event/KeyCodes.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/MouseCodes.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/Event.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/InputPoll.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/KeyEvent.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/MouseEvent.hpp


    ${VULKAN_INCLUDE_DIR}/Vulkan.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanWindow.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanDevice.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanPhysicalDevice.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanLogicalDevice.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanSwapchain.hpp

    # Renderer Includes
    ${ENGINE_INCLUDE_NAME}/Core/Renderer/Renderer.hpp
)

set(
    all_src
    ${ENGINE_SRC_DIR}/Core/ApplicationInstance.cpp


    ${ENGINE_SRC_DIR}/Core/platforms/main.cpp
    ${ENGINE_SRC_DIR}/Core/platforms/win32.cpp

    ${ENGINE_SRC_DIR}/Core/Window.cpp
    ${ENGINE_SRC_DIR}/Core/EngineLogger.cpp

    ${ENGINE_SRC_DIR}/Core/Event/InputPoll.cpp

    ${VULKAN_SRC_DIR}/Vulkan.cpp
    ${VULKAN_SRC_DIR}/VulkanWindow.cpp
    ${VULKAN_SRC_DIR}/VulkanDevice.cpp
    ${VULKAN_SRC_DIR}/VulkanPhysicalDevice.cpp
    ${VULKAN_SRC_DIR}/VulkanLogicalDevice.cpp
    ${VULKAN_SRC_DIR}/VulkanSwapchain.cpp

    ${ENGINE_SRC_DIR}/Core/Renderer/Renderer.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})