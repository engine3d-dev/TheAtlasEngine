module;

#include <vulkan/vulkan.h>

export module atlas.drivers.graphics_context;




export namespace atlas {

    class graphics_context {
    public:
        virtual ~graphics_context() = default;

        void destroy() {
            return destroy_context();
        }

    protected:
        virtual void destroy_context() = 0;
    };

};