module;

#include <unordered_map>
#include <functional>

export module atlas.core.utilities.poll_state;

export namespace atlas {
    inline std::unordered_map<void*, std::function<void(float)>> s_update{};
    inline std::unordered_map<void*, std::function<void()>> s_defer_update{};
    inline std::unordered_map<void*, std::function<void()>> s_ui_update{};
    inline std::unordered_map<void*, std::function<void()>> s_physica_update{};
    inline std::unordered_map<void*, std::function<void()>> s_start{};

    // TODO: Look into a different way of doing this
    void poll_update(void* p_address,
                     const std::function<void(float)>& p_callback) {
        s_update.emplace(p_address, p_callback);
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

    void poll_start(void* p_address, const std::function<void()>& p_callback) {
        s_start.emplace(p_address, p_callback);
    }

    // TEMP: This is a temporary solution, should look into doing this
    // differently
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
    void invoke_on_update(float p_delta_time) {
        for (auto& [address, on_update] : s_update) {
            on_update(p_delta_time);
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