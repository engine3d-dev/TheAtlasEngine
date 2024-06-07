#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Core/Window.h>

namespace Engine3D{
    static void GLFWErrorCallback(int error, const char* description){
        CoreLogError("GLFWErrorCallback MSG --- (Error Code -> {}): {}", error, description);
        assert(false);
    }

    Window* Window::Create(const WindowProperties& properties){
        return new Window(properties);
    }

    Window::Window(const WindowProperties& properties) : m_properties(properties) {

        //! @note Initializing OpenGL
        CoreLogWarn("Initializing Window");
        int success = glfwInit();

        if(!success){
            CoreLogError("Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        //! @note Hinting to OpenGL by specifying what OpenGL Version to enable
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

        m_windowHandler = glfwCreateWindow((int)m_properties.width, (int)m_properties.height, m_properties.title.c_str(), nullptr, nullptr);

        if(!m_windowHandler){
            CoreLogError("GLFWwindow handler was noticeable nullptr!");
            assert(false);
        }

        glfwMakeContextCurrent(m_windowHandler);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        CoreLogInfo(" OpenGL Version Information");
        CoreLogInfo(" Vendor: {0}", (const char *)glGetString(GL_VENDOR));
		CoreLogInfo(" Renderer: {0}", (const char *)glGetString(GL_RENDERER));
		CoreLogInfo(" Version: {0}", (const char *)glGetString(GL_VERSION));

        if(!status){
            CoreLogWarn("GLAD could not be loaded (Initialization Error!)");
            assert(false);
        }

        glfwSetWindowUserPointer(m_windowHandler, &m_properties);
        SetVSync(true);

        //! @note Event Handling
        // This is creating a WindowResizeEvent callback
        glfwSetWindowSizeCallback(m_windowHandler, [](GLFWwindow* window, int w, int h){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);
            data.width = w;
            data.height = h;

            // This is how we are going to be setting the callback from Application to event
            WindowResizeEvent event(w, h);
            data.callback(event);
        });

        // This is how we are dispatching the WindowCloseEvent callbacks to that specific event.
        glfwSetWindowCloseCallback(m_windowHandler, [](GLFWwindow* window){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.callback(event);
        });


        glfwSetKeyCallback(m_windowHandler, [](GLFWwindow* window, int key, int scancode, int action, int modes){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);

            // So when are doing a key call back the function takes in GLFWwindow, int, int, int
            //  action specified what action is occuring when with our key events, like are we pressing, releasing or repeating key events.
            // Event Usage: event(key, count) and for release is event(key)
            switch(action){
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(static_cast<KeyCode>(key));
                    data.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                    data.callback(event);
                    break;
                }
            }
        });

        // Callback for the Key Type Event
        // Essentially for typing text into a text box
        // NOTE: keycode is the character that we are currently typing
        glfwSetCharCallback(m_windowHandler, [](GLFWwindow* window, unsigned int keycode){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(static_cast<KeyCode>(keycode));
            data.callback(event);
        });

        // Setting up our event dispatchers for mouse events (ref to more a descriptive explanation at the top for key events as it applied to mouse events)
        glfwSetMouseButtonCallback(m_windowHandler, [](GLFWwindow* window, int button, int action, int modes){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);

            switch(action){
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                    data.callback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_windowHandler, [](GLFWwindow* window, double xOffset, double yOffset){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.callback(event);
        });

        glfwSetCursorPosCallback(m_windowHandler, [](GLFWwindow* window, double xPos, double yPos){
            WindowProperties& data =  *(WindowProperties *)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.callback(event);
        });
    }

    Window::~Window(){
        glfwDestroyWindow(m_windowHandler);
    }

    void Window::OnUpdate(){
        glfwSwapBuffers(m_windowHandler);
        glfwPollEvents();
    }

    void Window::SetName(const std::string& title){
        m_properties.title = title;
    }

    void Window::SetEventCallback(const EventCallbackFn& callback){
        m_properties.callback = callback;
    }

    void Window::Shutdown(){
        glfwDestroyWindow(m_windowHandler);
    }

    uint32_t Window::GetWidth() const{ return m_properties.width; }

    uint32_t Window::GetHeight() const { return m_properties.height; }

    void Window::SetVSync(bool enabled){
        if(enabled){
            glfwSwapInterval(1);
        }
        else{
            glfwSwapInterval(0);
        }

        m_properties.vsyncEnabled = enabled;
    }

    bool Window::IsVSyncEnabled() const {
        return m_properties.vsyncEnabled;
    }

    void* Window::GetNativeWindow() const {
        return m_windowHandler;
    }
};