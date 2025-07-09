#include <core/utilities/state.hpp>
#include <deque>

namespace atlas {
    namespace detail {
        inline std::deque<std::function<void()>> s_update{};
        inline std::deque<std::function<void()>> s_defer_update{};
        inline std::deque<std::function<void()>> s_ui_update{};
        inline std::deque<std::function<void()>> s_physica_update{};
        inline std::deque<std::function<void()>> s_start{};

        void poll_update(const std::function<void()>& p_callback) {
            s_update.emplace_back(p_callback);
        }

        void poll_defer_update(const std::function<void()>& p_callback) {
            s_defer_update.emplace_back(p_callback);
        }

        void poll_physics_update(const std::function<void()>& p_callback) {
            s_physica_update.emplace_back(p_callback);
        }

        void poll_ui_update(const std::function<void()>& p_callback) {
            s_ui_update.emplace_back(p_callback);
        }

        void poll_start(const std::function<void()>& p_callback) {
            s_start.emplace_back(p_callback);
        }

        void invoke_on_update() {
            for (auto& on_update : s_update) {
                on_update();
            }
        }

        void invoke_defer_update() {
            for (auto& on_update : s_defer_update) {
                on_update();
            }
        }

        void invoke_physics_update() {
            for (auto& on_update : s_physica_update) {
                on_update();
            }
        }

        void invoke_ui_update() {
            for (auto& on_update : s_ui_update) {
                on_update();
            }
        }

        void invoke_start() {
            for (auto& on_update : s_start) {
                on_update();
            }
        }
    };
};