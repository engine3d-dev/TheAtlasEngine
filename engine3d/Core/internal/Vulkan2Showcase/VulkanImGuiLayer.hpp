#pragma once

namespace engine3d::vk{
    class VulkanImGuiLayer{
    public:
        void Initialize();
        void BeginFrame();
        void EndFrame();
    };
};