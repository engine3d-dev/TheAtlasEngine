#pragma once
// #include <Core/internal/Core.h>
// #include <Core/core.h>
// #include <engine3d/Event/KeyCodes.h>
// #include <Core/Application.h>
#include <Core/Event/KeyCodes.h>
#include <Core/Event/MouseCodes.h>
#include <glm/glm.hpp>

namespace engine3d{
    class InputPoll{
    public:

        //! @note Key/Mouse event pressed!
        static bool IsKeyPressed(KeyCode keycode);

        static bool IsMousePressed(MouseCode mouseCode);

        //! @note Mouse Position
        static glm::vec2 GetMousePos();

        static float GetMouseX();

        static float GetMouseY();
    };
};