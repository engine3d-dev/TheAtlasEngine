#pragma once
namespace atlas::physics {
    class physics_context {
    public:
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