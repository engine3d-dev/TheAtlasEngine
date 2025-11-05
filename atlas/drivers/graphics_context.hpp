#pragma once
#include <core/core.hpp>

namespace atlas {
    /**
     * @brief graphics API-agnostic that is centralized to the graphics API themselves
    */
    class graphics_context {
    public:
        virtual ~graphics_context() = default;

        /**
         * @brief explicit cleanup of the graphics API context
        */
        void destroy() { return destroy_context(); }

    private:
        virtual void destroy_context() = 0;
    };

    /**
     * @brief construct a new graphics context and initializes that API
     * @return shared_ptr<graphics_context>
    */
    ref<graphics_context> initialize_context(const std::string& p_tag);

};