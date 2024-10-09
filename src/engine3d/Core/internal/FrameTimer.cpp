#include <Core/internal/FrameTimer.hpp>
#include <GLFW/glfw3.h>

namespace engine3d{
    static float g_EngineFrameTimer = 0.0f;
    static float g_LastEngineFrameTimer = 0.0f;

    void FrameTimer::UpdateFrameTimer(){
        g_LastEngineFrameTimer = (float)glfwGetTime();
        g_EngineFrameTimer = g_LastEngineFrameTimer;
    }

    float FrameTimer::GetSeconds(){
        return g_EngineFrameTimer;
    }

    float FrameTimer::GetMilliseconds(){ return g_EngineFrameTimer * 1000.0f; }

    
};