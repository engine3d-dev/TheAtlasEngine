set(ENGINE_INCLUDE_NAME engine3d)

include_directories(
    ../
    ../engine3d
)

set(
    all_headers
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/Image/stb_image.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/Image/Image.h

    # Event-base handlers
    ../${ENGINE_INCLUDE_NAME}/Core/Event/ApplicationEvent.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/Event.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/KeyEvent.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/MouseEvent.h
    ../${ENGINE_INCLUDE_NAME}/Core/Event/InputPoll.h

    # ====================================================================================
    # Third-Party Libraries
    # ====================================================================================
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/glad/glad.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/glad/KHR/khrplatform.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imconfig.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui_internal.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imstb_rectpack.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imstb_textedit.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imstb_truetype.h

    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_glfw.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_opengl2.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_opengl3_loader.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_opengl3.h
    ../${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_vulkan.h
    # ====================================================================================
    # ====================================================================================
    # ====================================================================================

    ../${ENGINE_INCLUDE_NAME}/Core/Layer.h
    ../${ENGINE_INCLUDE_NAME}/Core/EngineLogger.h
    ../${ENGINE_INCLUDE_NAME}/Core/Window.h

    # backend/Vulkan Sources
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Vulkan.h
    ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanDevice.h
    # ../${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanSurface.h
    # ../${ENGINE_INCLUDE_NAME}/Core/internal/platforms/Win32Application.h
)

set(
    all_src
    ${ENGINE_INCLUDE_NAME}/Core/backend/internal/Core.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Event/InputPoll.cpp
    
    # ====================================================================================
    # Third-Party Dependencies!
    # ====================================================================================
    ${ENGINE_INCLUDE_NAME}/Core/third_party/Image/stb_image.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/Image/Image.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/glad/glad.c
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui_demo.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui_tables.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui_draw.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/imgui_widgets.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_glfw.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_opengl3.cpp
    ${ENGINE_INCLUDE_NAME}/Core/third_party/imgui/backends/imgui_impl_vulkan.cpp

    # ====================================================================================
    # ====================================================================================
    # ====================================================================================

    # ====================================================================================
    # ====================================================================================
    # Actual Engine3D Sources
    # ====================================================================================
    # ====================================================================================
    
    ${ENGINE_INCLUDE_NAME}/Core/Layer.cpp
    # ${ENGINE_INCLUDE_NAME}/Core/internal/platforms/Win32Application.cpp
    ${ENGINE_INCLUDE_NAME}/Core/EngineLogger.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Window.cpp

    # backend stuff...
    ${ENGINE_INCLUDE_NAME}/Core/backend/utilities/helper_functions.cpp

    # backend/Vulkan Sources
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/Vulkan.cpp
    ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanDevice.cpp
    # ${ENGINE_INCLUDE_NAME}/Core/backend/Vulkan/VulkanSurface.cpp

)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})