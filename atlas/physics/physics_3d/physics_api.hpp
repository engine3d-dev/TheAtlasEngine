#pragma once

namespace atlas::physics {

    class physics_api {
        public:
            void update_jolt_values();
            void update_atlas_values();
            virtual ~physics_api() = default;

        private:
            virtual void retrieve_values() = 0;
            virtual void return_values() = 0;
    };
};