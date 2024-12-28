#pragma once
#include "api.hpp"
#include "drivers/swapchain.hpp"
#include <string>
#include <core/window.hpp>

namespace engine3d{
    class ApplicationInstance{
    public:
        ApplicationInstance(const std::string& p_Tag);
        void ExecuteMainloop();

        static Window& GetWindow() { return *s_ThisInstance->m_Window; }

        static API CurrentAPI();

        Ref<Swapchain> GetCurrentSwapchani();

        static ApplicationInstance* Super() { return s_ThisInstance; }

        static void ShutdownApplication();

    private:
        void SetCurrentAPI(API api);
    private:
        Ref<Window> m_Window;
        static ApplicationInstance* s_ThisInstance;
    };

    Ref<ApplicationInstance> Initialize();
};