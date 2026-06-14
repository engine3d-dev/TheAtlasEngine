module;

#include <print>
#include <unordered_map>
#include <functional>
#include <vector>

export module atlas.core.utilities:poll_state;

export namespace atlas {
    inline std::unordered_map<void*, std::vector<std::function<void(float)>>>
      s_update{};
    inline std::unordered_map<void*, std::vector<std::function<void()>>>
      s_defer_update{};
    inline std::unordered_map<void*, std::vector<std::function<void()>>>
      s_ui_update{};
    inline std::unordered_map<void*, std::vector<std::function<void()>>>
      s_physica_update{};
    inline std::unordered_map<void*, std::vector<std::function<void()>>>
      s_start{};
    inline std::unordered_map<void*, std::vector<std::function<void()>>>
      s_post{};

    void poll_update(void* p_address,
                     const std::function<void(float)>& p_callback) {
        s_update[p_address].emplace_back(p_callback);
    }

    void poll_defer_update(void* p_address,
                           const std::function<void()>& p_callback) {
        s_defer_update[p_address].emplace_back(p_callback);
    }

    void poll_physics_update(void* p_address,
                             const std::function<void()>& p_callback) {
        s_physica_update[p_address].emplace_back(p_callback);
    }

    void poll_ui_update(void* p_address,
                        const std::function<void()>& p_callback) {
        s_ui_update[p_address].emplace_back(p_callback);
    }

    void poll_start(void* p_address, const std::function<void()>& p_callback) {
        s_start[p_address].emplace_back(p_callback);
    }

    void post_update_poll(void* p_address,
                          const std::function<void()>& p_callback) {
        s_post[p_address].emplace_back(p_callback);
    }

    /**
     * @brief detail namespace is used for any internals that should not be
     * accessed by the user
     *
     * @note When switching to C++'s modules, hopefully this removes the
     * needs for having to represent namespaces in this way.
     *
     * Any invoke_* function is an internal detail that handles where those
     * behaviorial state callbacks get handled into their respective state
     * queue's
     *
     * Where the state queue will call those arbitrary callbacks at their
     * respective point in each of the frame.
     *
     * As thesse are intended for invoking those queue's directly.
     *
     */
    void invoke_on_update(void* p_scene, float p_delta_time) {
        for (auto& on_update : s_update[p_scene]) {
            on_update(p_delta_time);
        }
    }

    void invoke_defer_update(void* p_scene) {
        for (auto& on_update : s_defer_update[p_scene]) {
            on_update();
        }
    }

    void invoke_physics_update(void* p_scene) {
        for (auto& on_update : s_physica_update[p_scene]) {
            on_update();
        }
    }

    void invoke_ui_update(void* p_scene) {
        for (auto& on_update : s_ui_update[p_scene]) {
            on_update();
        }
    }

    void invoke_start(void* p_scene) {
        for (auto& on_update : s_start[p_scene]) {
            on_update();
        }
    }

    void invoke_post_update(void* p_scene) {
        for (auto& post_update : s_post[p_scene]) {
            post_update();
        }
    }
};