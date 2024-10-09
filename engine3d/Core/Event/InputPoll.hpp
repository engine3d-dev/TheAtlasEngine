#pragma once
#include <Event/KeyCodes.hpp>
#include <Event/MouseCodes.hpp>
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