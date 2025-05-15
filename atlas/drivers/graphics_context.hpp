#pragma once
#include <core/core.hpp>

namespace atlas {
    class graphics_context {
    public:
        virtual ~graphics_context() = default;
        void destroy();

    private:
        virtual void impl_destroy() = 0;
    };

    ref<graphics_context> initialize_context();
};