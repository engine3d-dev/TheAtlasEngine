#pragma once
#include <core/update_handlers/thread_manager.hpp>
#include <core/update_handlers/timer.hpp>

namespace atlas{
    //! TODO: global_update should be a namespace
    //! @note Rather then being a singleton-like class
    class global_update{
    public:
        /*
        * NEEDS FIX: Change to a more secure way to always have one
        * global_update per application instance
        */
        global_update() = delete;
        ~global_update() = delete;
        global_update(const global_update&) = delete;
        global_update& operator=(const global_update&) = delete;
        
        //! @note Initializes the global update manager and its cleanup function
        static void initialize();
        static void cleanup();

        // Manages the sync of each updateManager if required
        static void tick_update_frequency();

        // Maintains a const fps if possible
        static void wait_next_frame();

        static void IncrementCounter();

    private:

    };
};