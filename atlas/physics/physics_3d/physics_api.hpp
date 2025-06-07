#pragma once

namespace atlas::physics {

    class physics_api {
    public:
        /**
         * @brief Allows for flecs values to sync with jolt without running into
         * collisions.
         * @note Takes all transforms, rigidbodies, and collider bodies
         * that changed and updates jolt.
         *
         * @note should be called either by the user to sync events or
         * just before physics_update()
         */
        void update_jolt_values();

        /**
         * @brief Allows for jolt values to sync with flecs.
         *
         * @note should be called either by user to sync events or
         * just after physics_update()
         */
        void update_atlas_values();
        virtual ~physics_api() = default;

    private:
        /**
         * @brief Retrieves Flecs values for Jolt
         * 
         */
        virtual void retrieve_values() = 0;

        /**
         * @brief Returns jolt values to flecs
         * 
         */
        virtual void return_values() = 0;
    };
};