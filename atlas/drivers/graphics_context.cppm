module;

#include <vulkan/vulkan.h>
#include <functional>

export module atlas.drivers.graphics_context;




export namespace atlas {

    class graphics_context {
    public:
        virtual ~graphics_context() = default;

        void submit_resource_free(const std::function<void()>& p_resource) {
            return context_submit_resource_free(p_resource);
        }

        [[nodiscard]] VkInstance handle() const {
            return context_handle();
        }

        void destroy() {
            return destroy_context();
        }

    protected:
        virtual void destroy_context() = 0;
        virtual void context_submit_resource_free(const std::function<void()>& p_resource) = 0;

        [[nodiscard]] virtual VkInstance context_handle() const = 0;
    };

};