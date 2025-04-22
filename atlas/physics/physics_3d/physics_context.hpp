#pragma once

#include <physics\physics_3d\data\jolt_settings.hpp>
#include <physics\physics_3d\data\jolt_config.hpp>
namespace atlas::physics {
    class physics_context {
        public:
            physics_context(const jolt_settings settings);
            void runtime_start();
            void runtime_end();

        private:
            virtual void engine_runtime_start() = 0;
            virtual void engine_runtime_end() = 0;

        
    };
};