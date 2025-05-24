#pragma once
#include <core/core.hpp>

namespace atlas {
    class graphics_context {
    public:
        virtual ~graphics_context() = default;
        
        void destroy() { return destroy_context(); }

    private:
        virtual void destroy_context() = 0;
    };

    ref<graphics_context> initialize_context(const std::string& p_tag);
    
};