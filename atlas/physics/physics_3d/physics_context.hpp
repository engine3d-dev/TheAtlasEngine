#pragma once
namespace atlas::physics {
    /**
     * @brief The context is the way to interact with only the engine. It is the
     * api for all the background funcitons and information that the user
     * shouldn not see. It is a virtual based type erasure class so thatmany
     * engines can implement the same functions. Specifically realted to backend
     * engine creation like batching, starting runtime, etc...
     *
     */
    class physics_context {
    public:
        // Pass through function to allow private virtual functions to be called
        // publically without messing up the virtual table.
        void create_bodies();
        void clean_bodies();
        void run_physics_step();
        void contact_added_event();

        //! @remark This has to be public to get shared pointer working
        //! @note With one exception being some friend class but not the way
        virtual ~physics_context() = default;

        /**
         * @note:
         *  Many more in the future like:
         *  batching physics bodies
         *  chucking engine location
         *  shifting coordinates
         */

    private:
        virtual void engine_create_physics_bodies() = 0;
        virtual void engine_clean_physics_bodies() = 0;
        virtual void engine_run_physics_step() = 0;
        virtual void engine_run_contact_added() = 0;
    };
};