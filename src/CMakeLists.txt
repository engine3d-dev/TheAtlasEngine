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
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Vulkan.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanDevice.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanSwapchain.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanCommandBuffer.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanCommandBuffer.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanRenderPass.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderModule.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h

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
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Vulkan.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanDevice.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanSwapchain.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanCommandBuffer.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanCommandQueue.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanRenderPass.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderModule.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Renderer/Renderer.cpp


    # Application instance 
    ${ENGINE_INCLUDE_NAME}/Core/ApplicationInstance.cpp


    # Targeted platforms
    ${ENGINE_INCLUDE_NAME}/Core/platforms/main.cpp
    ${ENGINE_INCLUDE_NAME}/Core/platforms/win32.cpp
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})