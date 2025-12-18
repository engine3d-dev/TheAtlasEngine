#include <core/utilities/state.hpp>
#include <unordered_map>

namespace atlas {
    namespace detail {
        // inline std::deque<std::function<void()>> s_update{};
        // inline std::deque<std::function<void()>> s_defer_update{};
        // inline std::deque<std::function<void()>> s_ui_update{};
        // inline std::deque<std::function<void()>> s_physica_update{};
        // inline std::deque<std::function<void()>> s_start{};

        // TODO: This should be done in a better way
        // Potential replace this approach with using std::hive from C++26
        inline std::unordered_map<void*, std::function<void()>> s_update{};
        inline std::unordered_map<void*, std::function<void()>>
          s_defer_update{};
        inline std::unordered_map<void*, std::function<void()>> s_ui_update{};
        inline std::unordered_map<void*, std::function<void()>>
          s_physica_update{};
        inline std::unordered_map<void*, std::function<void()>> s_start{};

        void poll_update(void* p_address,
                         const std::function<void()>& p_callable) {
            // s_update.emplace_back(p_callback);
            s_update.emplace(p_address, p_callable);
        }

        void poll_defer_update(void* p_address,
                               const std::function<void()>& p_callback) {
            s_defer_update.emplace(p_address, p_callback);
        }

        void poll_physics_update(void* p_address,
                                 const std::function<void()>& p_callback) {
            s_physica_update.emplace(p_address, p_callback);
        }

        void poll_ui_update(void* p_address,
                            const std::function<void()>& p_callback) {
            s_ui_update.emplace(p_address, p_callback);
        }

        void poll_start(void* p_address,
                        const std::function<void()>& p_callback) {
            s_start.emplace(p_address, p_callback);
        }

        void remove_update(void* p_address) {
            s_update.erase(p_address);
        }

        void remove_defer_update(void* p_address) {
            s_defer_update.erase(p_address);
        }

        void remove_physics_update(void* p_address) {
            s_physica_update.erase(p_address);
        }

        void remove_ui_update(void* p_address) {
            s_ui_update.erase(p_address);
        }

        void remove_start(void* p_address) {
            s_start.erase(p_address);
        }

        void invoke_on_update() {
            for (auto& [address, on_update] : s_update) {
                on_update();
            }
        }

        void invoke_defer_update() {
            for (auto& [address, on_update] : s_defer_update) {
                on_update();
            }
        }

        void invoke_physics_update() {
            for (auto& [address, on_update] : s_physica_update) {
                on_update();
            }
        }

        void invoke_ui_update() {
            for (auto& [address, on_update] : s_ui_update) {
                on_update();
            }
        }

        void invoke_start() {
            for (auto& [address, on_update] : s_start) {
                on_update();
            }
        }
    };
};