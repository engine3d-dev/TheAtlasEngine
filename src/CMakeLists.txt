set(ENGINE_INCLUDE_NAME engine3d)

include_directories(
    ../
    ../engine3d
)

set(
    all_src
    ${ENGINE_INCLUDE_NAME}/Core/EntryPoint.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Application.cpp
    ${ENGINE_INCLUDE_NAME}/Core/EngineLogger.cpp

    ${ENGINE_INCLUDE_NAME}/Event/InputPoll.cpp
    ${ENGINE_INCLUDE_NAME}/Core/Window.cpp

    ${ENGINE_INCLUDE_NAME}/Core/Layer.cpp
    ${ENGINE_INCLUDE_NAME}/Core/LayerStack.cpp

    ${ENGINE_INCLUDE_NAME}/ImGui/ImGuiLayer.cpp
    ${ENGINE_INCLUDE_NAME}/Core/UI/UILayer.cpp

    ${ENGINE_INCLUDE_NAME}/Core/UI/UILayer.cpp
    ${ENGINE_INCLUDE_NAME}/Image/stb_image.cpp

    ${ENGINE_INCLUDE_NAME}/Image/stb_image.cpp
    ${ENGINE_INCLUDE_NAME}/Image/Image.cpp

    # glad.c
    ${ENGINE_INCLUDE_NAME}/Core/glad/glad.c

    # imgui src
    ${ENGINE_INCLUDE_NAME}/Core/imgui/imgui_demo.cpp
    ${ENGINE_INCLUDE_NAME}/Core/imgui/imgui_tables.cpp
    ${ENGINE_INCLUDE_NAME}/Core/imgui/imgui.cpp
    ${ENGINE_INCLUDE_NAME}/Core/imgui/imgui_draw.cpp
    ${ENGINE_INCLUDE_NAME}/Core/imgui/imgui_widgets.cpp
    ${ENGINE_INCLUDE_NAME}/Core/imgui/backends/imgui_impl_glfw.cpp
    ${ENGINE_INCLUDE_NAME}/Core/imgui/backends/imgui_impl_opengl3.cpp


    # Graphics Includes
    ${ENGINE_INCLUDE_NAME}/Graphics/Pipeline.cpp
    ${ENGINE_INCLUDE_NAME}/Graphics/Buffer.cpp
    ${ENGINE_INCLUDE_NAME}/Graphics/VertexArray.cpp
    ${ENGINE_INCLUDE_NAME}/Graphics/Shader.cpp
    ${ENGINE_INCLUDE_NAME}/Graphics/Texture.cpp
    ${ENGINE_INCLUDE_NAME}/Graphics/Framebuffer.cpp

    # OpenGL src files
    ${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLBuffer.cpp
    ${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLVertexArray.cpp
    ${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLShader.cpp
    ${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLTexture.cpp
    ${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLFramebuffer.cpp

    # Scene sources
    ${ENGINE_INCLUDE_NAME}/Renderer/Renderer.cpp
    ${ENGINE_INCLUDE_NAME}/Scene/OrthographicCameraController.cpp

)

set(
    all_headers
    ../${ENGINE_INCLUDE_NAME}/engine3dPrecompiledHeader.h
    ../${ENGINE_INCLUDE_NAME}/Core/core.h
    ../${ENGINE_INCLUDE_NAME}/Core/Application.h
    ../${ENGINE_INCLUDE_NAME}/Core/EngineLogger.h
    ../${ENGINE_INCLUDE_NAME}/Core/EntryPoint.h
    ../${ENGINE_INCLUDE_NAME}/Image/stb_image.h
    ../${ENGINE_INCLUDE_NAME}/Image/Image.h

    ../${ENGINE_INCLUDE_NAME}/Core/Layer.h
    ../${ENGINE_INCLUDE_NAME}/Core/LayerStack.h

    ../${ENGINE_INCLUDE_NAME}/ImGui/ImGuiLayer.h
    ../${ENGINE_INCLUDE_NAME}/Core/UI/UILayer.h

    # Event handlers
    ../${ENGINE_INCLUDE_NAME}/Event/ApplicationEvent.h
    ../${ENGINE_INCLUDE_NAME}/Event/Event.h
    ../${ENGINE_INCLUDE_NAME}/Event/KeyEvent.h
    ../${ENGINE_INCLUDE_NAME}/Event/MouseEvent.h
    ../${ENGINE_INCLUDE_NAME}/Event/InputPoll.h

    # glad includes
    ../${ENGINE_INCLUDE_NAME}/Core/glad/glad.h
    ../${ENGINE_INCLUDE_NAME}/Core/KHR/khrplatform.h

    # imgui includes
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/imconfig.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/imgui_internal.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/imgui.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/imstb_rectpack.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/imstb_textedit.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/imstb_truetype.h

    ../${ENGINE_INCLUDE_NAME}/Core/imgui/backends/imgui_impl_glfw.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/backends/imgui_impl_opengl2.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/backends/imgui_impl_opengl3_loader.h
    ../${ENGINE_INCLUDE_NAME}/Core/imgui/backends/imgui_impl_opengl3.h

    ../${ENGINE_INCLUDE_NAME}/Graphics/Pipeline.h
    ../${ENGINE_INCLUDE_NAME}/Graphics/Buffer.h
    ../${ENGINE_INCLUDE_NAME}/Graphics/VertexArray.h
    ../${ENGINE_INCLUDE_NAME}/Graphics/Shader.h
    ../${ENGINE_INCLUDE_NAME}/Graphics/Texture.h
    ../${ENGINE_INCLUDE_NAME}/Graphics/Framebuffer.h

    ../${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLBuffer.h
    ../${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLVertexArray.h
    ../${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLShader.h
    ../${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLTexture.h
    ../${ENGINE_INCLUDE_NAME}/OpenGL/OpenGLFramebuffer.h

    ../${ENGINE_INCLUDE_NAME}/Renderer/Renderer.h
    ../${ENGINE_INCLUDE_NAME}/Scene/OrthographicCameraController.h
)

add_library(${PROJECT_NAME} ${all_headers} ${all_src})