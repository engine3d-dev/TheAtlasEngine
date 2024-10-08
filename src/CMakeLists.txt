set(ENGINE_INCLUDE_NAME ../engine3d)
set(ENGINE_SRC_DIR engine3d)
set(VULKAN_INCLUDE_DIR ${ENGINE_INCLUDE_NAME}/Core/internal/VulkanCpp)
set(VULKAN_SRC_DIR ${ENGINE_SRC_DIR}/Core/internal/VulkanCpp)

set(
    all_headers

    ${ENGINE_INCLUDE_NAME}/Core/ApplicationInstance.h

    ${ENGINE_INCLUDE_NAME}/Core/EngineLogger.h
    ${ENGINE_INCLUDE_NAME}/Core/Window.h

    ${VULKAN_INCLUDE_DIR}/Vulkan.h
    ${VULKAN_INCLUDE_DIR}/VulkanWindow.h
    ${VULKAN_INCLUDE_DIR}/VulkanDevice.h
    ${VULKAN_INCLUDE_DIR}/VulkanPhysicalDevice.h
    ${VULKAN_INCLUDE_DIR}/VulkanLogicalDevice.h
    ${VULKAN_INCLUDE_DIR}/VulkanSwapchain.h

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

    ${VULKAN_SRC_DIR}/Vulkan.cpp
    ${VULKAN_SRC_DIR}/VulkanWindow.cpp
    ${VULKAN_SRC_DIR}/VulkanDevice.cpp
    ${VULKAN_SRC_DIR}/VulkanPhysicalDevice.cpp
    ${VULKAN_SRC_DIR}/VulkanLogicalDevice.cpp
    ${VULKAN_SRC_DIR}/VulkanSwapchain.cpp

    ${ENGINE_SRC_DIR}/Core/Renderer/Renderer.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})