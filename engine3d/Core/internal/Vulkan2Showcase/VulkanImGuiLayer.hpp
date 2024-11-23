#pragma once

namespace engine3d::vk{
    class VulkanImGuiLayer{
    public:
        static void Initialize();
        static void BeginFrame();
        static void EndFrame();
    };
};