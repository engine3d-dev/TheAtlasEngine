#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Event/KeyCodes.h>
#include <engine3d/Event/MouseCodes.h>
#include "glm/glm.hpp"

namespace Engine3D{
    class ENGINE_API InputPoll{
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