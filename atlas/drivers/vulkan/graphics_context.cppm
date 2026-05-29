module;

#include <optional>
#include <memory>
#include <functional>
#include <deque>

#include <print>

#define GLFW_INCLUDE_VULKAN
#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif

export module atlas.drivers.vulkan:graphics_context;

import vk;

export namespace atlas {

    class graphics_context {
    public:
        graphics_context() = default;
        graphics_context(const vk::instance& p_api_instance,
                         vk::physical_device& p_physical,
                         std::shared_ptr<vk::device> p_device)
          : m_api_instance(p_api_instance)
          , m_physical(&p_physical)
          , m_device(p_device) {
            // Constructing the graphics context

            s_instance = this;
        }

        ~graphics_context() = default;

        void post_cleanup() {
            for (const auto& callback : m_submit_resource_free) {
                callback();
            }
        }

        [[nodiscard]] vk::instance instance_handle() const {
            return m_api_instance;
        }

        [[nodiscard]] vk::physical_device physical_device() const {
            return *m_physical;
        }

        std::shared_ptr<vk::device> logical_device() { return m_device; }

        static void submit_resource_free(
          const std::function<void()>& p_callback) {
            s_instance->m_submit_resource_free.emplace_back(p_callback);
        }

    private:
        vk::instance m_api_instance;
        vk::physical_device* m_physical = nullptr;
        std::shared_ptr<vk::device> m_device = nullptr;
        std::deque<std::function<void()>> m_submit_resource_free{};
        static graphics_context* s_instance;
    };

    graphics_context* graphics_context::s_instance = nullptr;

};