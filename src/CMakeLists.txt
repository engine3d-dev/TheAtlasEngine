set(ENGINE_INCLUDE_NAME engine3d)

set(
    all_headers
    ../${ENGINE_INCLUDE_NAME}/Core/backend/third_party/Image/stb_image.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/third_party/Image/Image.h

    # Event-base handlers
    ../${ENGINE_INCLUDE_NAME}/Core/Event/ApplicationEvent.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/Event.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/KeyEvent.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/MouseEvent.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/InputPoll.h

    ../${ENGINE_INCLUDE_NAME}/Core/backend/internal/FrameTimer.h

    # Application includes
    ../${ENGINE_INCLUDE_NAME}/Core/ApplicationInstance.h

    # UI Includes
    ../${ENGINE_INCLUDE_NAME}/Core/UI/UICore.h

    # ====================================================================================
    # Third-Party Libraries
    # ====================================================================================

    ../${ENGINE_INCLUDE_NAME}/Core/Layer.h
    ../${ENGINE_INCLUDE_NAME}/Core/EngineLogger.h
    ../${ENGINE_INCLUDE_NAME}/Core/Window.h

    # backend/Vulkan Sources
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/helper-functions.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/Vulkan.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanDevice.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanSwapchain.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanCommandBuffer.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanCommandBuffer.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanRenderPass.h
    # ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderModule.h
    # ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h

    ../${ENGINE_INCLUDE_NAME}/Core/Renderer/Renderer.h
    ../${ENGINE_INCLUDE_NAME}/Core/Engine/Engine3D.h
)

set(
    all_src
    ${ENGINE_INCLUDE_NAME}/Core/backend/internal/Core.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/InputPoll.cpp

    ${ENGINE_INCLUDE_NAME}/Core/backend/internal/FrameTimer.cpp
    
    # ====================================================================================
    # Third-Party Dependencies!
    # ====================================================================================
    ${ENGINE_INCLUDE_NAME}/Core/backend/third_party/Image/stb_image.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/third_party/Image/Image.cpp
    # ${ENGINE_INCLUDE_NAME}/Core/backend/third_party/glad/glad.c
    # ====================================================================================
    # ====================================================================================
    # Actual Engine3D Sources
    # ====================================================================================
    # ====================================================================================
    
    ${ENGINE_INCLUDE_NAME}/Core/Layer.cpp
    
    ${ENGINE_INCLUDE_NAME}/Core/UI/UICore.cpp

    # ${ENGINE_INCLUDE_NAME}/Core/internal/platforms/Win32Application.cpp
    ${ENGINE_INCLUDE_NAME}/Core/EngineLogger.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Window.cpp

    # backend stuff...
    ${ENGINE_INCLUDE_NAME}/Core/backend/utilities/helper_functions.cpp

    # backend/Vulkan Sources
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/helper-functions.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/Vulkan.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanDevice.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanSwapchain.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanCommandBuffer.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanCommandQueue.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan-Experiemental/VulkanRenderPass.cpp
    # ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderModule.cpp
    # ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Renderer/Renderer.cpp


    # Application instance 
    ${ENGINE_INCLUDE_NAME}/Core/ApplicationInstance.cpp


    # Targeted platforms
    ${ENGINE_INCLUDE_NAME}/Core/platforms/main.cpp
    ${ENGINE_INCLUDE_NAME}/Core/platforms/win32.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})