module;

#include <GLFW/glfw3.h>

export module atlas.core.event;


export namespace atlas {
    void flush_events() {
        glfwPollEvents();
    }
};