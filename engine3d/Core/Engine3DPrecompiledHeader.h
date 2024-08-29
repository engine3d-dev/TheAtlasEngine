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
// #ifndef __gl__h
// #include <glad/glad.h>
// #include <Core/glad/glad.h>
// #endif
// #include <Core/backend/third_party/glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/ostream.h>

// #include <imgui/imgui.h>
// #include <imgui/backends/imgui_impl_opengl3.h>
// #include <imgui/backends/imgui_impl_glfw.h>
// #include <Core/backend/third_party/imgui/imgui.h>
// #include <Core/backend/third_party/imgui/backends/imgui_impl_opengl3.h>
// #include <Core/backend/third_party/imgui/backends/imgui_impl_glfw.h>
// #include <ImGuizmo/ImGuizmo.h>
// #include <Core/backend/third_party/ImGuizmo/ImGuizmo.h>

//! @note Physics-related includes
// @note box2D related includes
// #include <box2d/box2d.h>
// #include <box2d/b2_body.h>
// #include <box2d/b2_fixture.h>
// #include <box2d/b2_polygon_shape.h>
#include <yaml-cpp/yaml.h>

//! @note Engine3D Includes
// #include <Core/Application.h>
// #include <Core/EngineLogger.h>
#include <Core/Event/InputPoll.h>
// #include <Core/Timer.h>
// #include <Core/Timestep.h>
#include <Core/backend/third_party/Image/stb_image.h>
#include <Core/backend/third_party/Image/Image.h>

#include <Core/Event/Event.h>
#include <Core/Event/KeyEvent.h>
#include <Core/Event/MouseEvent.h>
#include <Core/Event/ApplicationEvent.h>