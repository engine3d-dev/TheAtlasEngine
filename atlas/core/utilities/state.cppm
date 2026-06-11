module;

#include <type_traits>
#include <cstring>

export module atlas.core.utilities.state;

import atlas.core.utilities.poll_state;

export namespace atlas {
    /**
     * @brief preloading any behavior that may be required of the users game
     * objects, such as pre-loading assets or any metadata after construction of
     * the objects. During engine's preloading stage.
     *
     * This is very useful for spawning or not initiating any pre-load logic
     * that does not happen during the construction of your object itself
     *
     * Ideally I would like to expand this to allow for level transitioning of
     * pointing to start registration for transitioning various preloading logic
     *
     * @tparam UObject is the specific object to point to the specified callback
     * @tparam UCallback is the arbitrary type of the callback that contains the
     * game logic itself
     * @param p_instance is a pointer that points where the callback is coming
     * from
     * @param p_callable is the callback of arbitrary logic that then gets
     * executed and is used alongside p_instance
     *
     * Example Usage:
     *
     * ```C++
     * atlas::register_start(this, &level_scene::preload);
     * ```
     */
    template<typename UObject, typename UCallback>
    void register_start(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        poll_start(p_instance,
                   [p_instance, p_callable]() { (p_instance->*p_callable)(); });
    }

    /**
     * @brief Used for executing user-defined game logic during every framerate.
     *
     * This API allows you to register your game logic to be executed during
     * every frame call.
     *
     * This is useful for any kind of game logic that is related to game inputs.
     *
     * @tparam UObject is the specific object to point to the specified callback
     * @tparam UCallback is the arbitrary type of the callback that contains the
     * game logic itself
     * @param p_instance is a pointer that points where the callback is coming
     * from
     * @param p_callable is the callback of arbitrary logic that then gets
     * executed and is used alongside p_instance
     *
     * Example Usage:
     *
     * ```C++
     * atlas::register_update(this, &level_scene::update_logic);
     * ```
     */
    template<typename UObject, typename UCallback>
    void register_update(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        poll_update(p_instance, [p_instance, p_callable](float p_delta_time) {
            (p_instance->*p_callable)(p_delta_time);
        });
    }

    /**
     * @brief Primarily used for executing physics game logic during the physics
     * fixed framerate
     *
     * This API rather allows for explicit control over where to execute your
     * callback to the physics fixed framerate
     * @tparam UObject is the specific object to point to the specified callback
     * @tparam UCallback is the arbitrary type of the callback that contains the
     * game logic itself
     * @param p_instance is a pointer that points where the callback is coming
     * from
     * @param p_callable is the callback of arbitrary logic that then gets
     * executed and is used alongside p_instance
     *
     * Example Usage:
     *
     * ```C++
     * atlas::register_physics(this, &level_scene::on_physics);
     * ```
     */
    template<typename UObject, typename UCallback>
    void register_physics(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        poll_physics_update(p_instance, [p_instance, p_callable]() {
            (p_instance->*p_callable)();
        });
    }

    /**
     * @brief Intended to be used to specify game logic to be executed at the
     * last possible moment in your game
     *
     * Intended to be used for game object logic that needs to be resolved at
     * the last-moment in the frame.
     *
     * @tparam UObject is the specific object to point to the specified callback
     * @tparam UCallback is the arbitrary type of the callback that contains the
     * game logic itself
     * @param p_instance is a pointer that points where the callback is coming
     * from
     * @param p_callable is the callback of arbitrary logic that then gets
     * executed and is used alongside p_instance
     *
     * Example Usage:
     *
     * ```C++
     * atlas::register_deferred(this, &level_scene::late_update);
     * ```
     *
     */
    template<typename UObject, typename UCallback>
    void register_deferred(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        poll_defer_update(p_instance, [p_instance, p_callable]() {
            (p_instance->*p_callable)();
        });
    }

    /**
     * @brief This is just for registering UI logic for handling editorial UI
     * logic such as widgets modification to entity transform components
     *
     * Intended for containing logic widgets may be used in.
     *
     * TODO: This might be redundant and may actually consider either removing
     * this or simplify by just using register_deferred instead.
     *
     * @tparam UObject is the specific object to point to the specified callback
     * @tparam UCallback is the arbitrary type of the callback that contains the
     * game logic itself
     * @param p_instance is a pointer that points where the callback is coming
     * from
     * @param p_callable is the callback of arbitrary logic that then gets
     * executed and is used alongside p_instance
     *
     * Example Usage:
     *
     * ```C++
     * atlas::register_ui(this, &level_scene::late_update);
     * ```
     *
     */
    template<typename UObject, typename UCallback>
    void register_ui(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        poll_ui_update(p_instance, [p_instance, p_callable]() {
            (p_instance->*p_callable)();
        });
    }


    template<typename UObject, typename UCallback>
    void register_post(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        post_update_poll(p_instance, [p_instance, p_callable]() {
            (p_instance->*p_callable)();
        });
    }
};