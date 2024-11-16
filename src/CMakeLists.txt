set(ENGINE_INCLUDE_NAME ../engine3d)
set(ENGINE_INTERNAL_INCLUDE ${ENGINE_INCLUDE_NAME}/Core/internal)
set(ENGINE_SRC_DIR engine3d)
set(VULKAN_INCLUDE_DIR ${ENGINE_INCLUDE_NAME}/Core/internal/Vulkan2Showcase)
set(VULKAN_SRC_DIR ${ENGINE_SRC_DIR}/Core/internal/Vulkan2Showcase)

set(VULKAN_SHADERS_INCLUDE_DIR ${VULKAN_INCLUDE_DIR}/Shaders)
set(VULKAN_SHADERS_SRC_DIR ${VULKAN_SRC_DIR}/Shaders)

build_library(
    SOURCES

    ${ENGINE_INCLUDE_NAME}/Core/ApplicationInstance.hpp

    ${ENGINE_INCLUDE_NAME}/Core/EngineLogger.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Window.hpp
    # ${ENGINE_INCLUDE_NAME}/Core/GraphicDrivers/Swapchain.hpp

    # Event system includes
    ${ENGINE_INCLUDE_NAME}/Core/Event/KeyCodes.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/MouseCodes.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/Event.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/InputPoll.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/KeyEvent.hpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/MouseEvent.hpp

    ${VULKAN_INCLUDE_DIR}/Vulkan.hpp
    ${VULKAN_INCLUDE_DIR}/helper_functions.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanContext.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanWindow.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanPhysicalDriver.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanDriver.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanSwapchain.hpp
    ${VULKAN_SHADERS_INCLUDE_DIR}/VulkanShader.hpp
    ${VULKAN_INCLUDE_DIR}/VulkanVertexBuffer.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanPhysicalDriver.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanDevice.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanPhysicalDevice.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanLogicalDevice.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanAllocator.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanSwapchain.hpp
    # ${VULKAN_INCLUDE_DIR}/VulkanRenderer.hpp
    # ${VULKAN_INCLUDE_DIR}/utilities.hpp

    # Renderer Includes
    ${ENGINE_INCLUDE_NAME}/Core/Renderer/Renderer.hpp

    ${ENGINE_SRC_DIR}/Core/ApplicationInstance.cpp


    ${ENGINE_SRC_DIR}/Core/platforms/main.cpp
    ${ENGINE_SRC_DIR}/Core/platforms/win32.cpp

    ${ENGINE_SRC_DIR}/Core/Window.cpp
    # ${ENGINE_SRC_DIR}/Core/GraphicDrivers/Swapchain.cpp
    ${ENGINE_SRC_DIR}/Core/EngineLogger.cpp

    ${ENGINE_SRC_DIR}/Core/Event/InputPoll.cpp

    ${VULKAN_SRC_DIR}//helper_functions.cpp
    ${VULKAN_SRC_DIR}/VulkanWindow.cpp
    ${VULKAN_SRC_DIR}/VulkanContext.cpp
    ${VULKAN_SRC_DIR}/VulkanPhysicalDriver.cpp
    ${VULKAN_SRC_DIR}/VulkanDriver.cpp
    ${VULKAN_SRC_DIR}/VulkanSwapchain.cpp

    ${VULKAN_SHADERS_SRC_DIR}/VulkanShader.cpp

    ${VULKAN_SRC_DIR}/VulkanVertexBuffer.cpp
    # ${VULKAN_SRC_DIR}/VulkanPhysicalDriver.cpp
    # ${VULKAN_SRC_DIR}/VulkanWindow.cpp
    # ${VULKAN_SRC_DIR}/VulkanDevice.cpp
    # ${VULKAN_SRC_DIR}/VulkanPhysicalDevice.cpp
    # ${VULKAN_SRC_DIR}/VulkanLogicalDevice.cpp
    # ${VULKAN_SRC_DIR}/VulkanAllocator.cpp
    # ${VULKAN_SRC_DIR}/VulkanSwapchain.cpp
    # ${VULKAN_SRC_DIR}/VulkanRenderer.cpp
    # ${VULKAN_SRC_DIR}/utilities.cpp

    ${ENGINE_SRC_DIR}/Core/Renderer/Renderer.cpp
)