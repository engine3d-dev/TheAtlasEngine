#pragma once
#include <Core/Core.h>
#include <string>
#include <GLFW/glfw3.h>

namespace engine3d{
    struct WindowProperties{
        std::string title="Default";
        uint32_t width=0, height=0;
    };

    class Window{
    public:
        Window(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title);
        // GLFWwindow* NativeWindow();

        /**
         * @name As(typename input);
         * @param UWindowSpecified represents the windowing system to convert to
         * @param IWidnowSpecified is what the user defines as their window.
         * @note TODO --- Probably would have this be in helper_functions.h as this is an exposed to the API that uses may used to get the right API.
        */
        template<typename UWindowSpecified, typename IWindowSpecified>
        UWindowSpecified* As(IWindowSpecified){
            return static_cast<UWindowSpecified *>(GetCurrentWindowAPI());
        }

        bool IsWindowShutdown() const;

        //! @note Used for updating/presenting our images to the display per frame basis.
        void OnUpdateDisplay();

        static WindowProperties GetProperties();

    protected:
        virtual void UpdateFrame() = 0;

    private:
        //! @note Returns our current set native window API.
        virtual void* GetCurrentWindowAPI();
    public:
        GLFWwindow* m_WindowInstance;
    };
};