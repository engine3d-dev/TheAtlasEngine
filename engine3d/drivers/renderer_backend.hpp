#pragma once
#include <string>
#include <core/core.hpp>

namespace engine3d{
    class RendererContext{
    public:
        static Scope<RendererContext> Initialize(const std::string& p_Tag);
        virtual ~RendererContext() = default;

        void Begin();
        void End();
        
    private:
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
    };
};