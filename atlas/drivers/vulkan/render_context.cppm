module;

#include <memory>
#include <print>

export module atlas.drivers.vulkan:render_context;

import :graphics_context;
import vk;

export namespace atlas {

    /**
     * @brief Core render context to schedule images and barriers for coordinating rendering operations
     * 
     * Manages multiple render contexts
    */
    class render_context {
    public:
        render_context() = default;
        render_context(std::shared_ptr<graphics_context> p_context) {
            m_device = p_context->logical_device();

            std::println("Constructing render_context(shared_ptr<graphics_context>)");
        }


        void destruct() {
        }
    
    private:
        std::shared_ptr<vk::device> m_device;
    };
};