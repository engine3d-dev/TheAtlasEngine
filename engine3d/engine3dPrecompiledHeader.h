#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <cstdarg>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>

#include <array>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>

// OpenGL includes
#ifndef __gl__h
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/ostream.h>

// #include <imgui/imgui.h>
// #include <imgui/backends/imgui_impl_opengl3.h>
// #include <imgui/backends/imgui_impl_glfw.h>
#include <engine3d/Core/imgui/imgui.h>
#include <engine3d/Core/imgui/backends/imgui_impl_opengl3.h>
#include <engine3d/Core/imgui/backends/imgui_impl_glfw.h>
// #include <ImGuizmo/ImGuizmo.h>

//! @note Physics-related includes
// @note box2D related includes
// #include <box2d/box2d.h>
// #include <box2d/b2_body.h>
// #include <box2d/b2_fixture.h>
// #include <box2d/b2_polygon_shape.h>
#include <yaml-cpp/yaml.h>

//! @note Engine3D Includes
#include <engine3d/Core/Application.h>
#include <engine3d/Core/EngineLogger.h>
#include <engine3d/Event/InputPoll.h>
#include <engine3d/Core/Timer.h>
#include <engine3d/Core/Timestep.h>
#include <engine3d/Image/stb_image.h>
#include <engine3d/Image/Image.h>


// //! @note Graphics interfaces
#include <engine3d/Graphics/Buffer.h>
#include <engine3d/Graphics/VertexArray.h>
#include <engine3d/Graphics/Shader.h>
#include <engine3d/Graphics/Texture.h>

#include <engine3d/Graphics/Pipeline.h>
#include <engine3d/Renderer/Renderer.h>

#include <engine3d/Event/Event.h>
#include <engine3d/Event/KeyEvent.h>
#include <engine3d/Event/MouseEvent.h>
#include <engine3d/Event/ApplicationEvent.h>