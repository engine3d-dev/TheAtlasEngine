#pragma once
#include <string>
#include <core/scene/world.hpp>
#include <flecs.h>

namespace atlas {
    /**
     * @name Entity
     * @note Wrapper around specifically creating a type of entity wrapper
     * around the flecs API
     * @note It provides the same mechanism for adding, removing, and checking
     * if components are stored.
     * @note Flecs API specified that querying components are how they
     * find/iterate through entities
     *       - Queries are also used throughout parts of the API such as their
     * systems and observers
     */
    class entity_t {
    public:
        entity_t() = default;
        entity_t(flecs::world* p_registry, const std::string& p_tag);
        entity_t(const flecs::entity& p_entity)
          : m_entity_id(p_entity) {}

        ~entity_t();

        //! @note Flecs allows to check when an entity has been created
        //! @note Knowing when this entity lifecyle's ended
        [[nodiscard]] bool is_alive() const { return m_entity_id.is_alive(); }

        void on_destruction() { m_entity_id.destruct(); }

        //! @note Creates component in the ECS storage but does not assign it
        //! with a value.
        template<typename UComponent>
        void add() {
            m_entity_id.add<UComponent>();
        }

        //! @note Returns the component from the entity
        //! @note Flecs has you return a pointer for checking if the component
        //! is found
        //! @note If not found will return nullptr
        template<typename UComponent>
        const UComponent* get() const {
            return m_entity_id.get<UComponent>();
        }

        template<typename UComponent>
        UComponent* get_mut() const {
            return m_entity_id.get_mut<UComponent>();
        }

        //! @note Checks if specific component of type UComponent is provided to
        //! this entity
        template<typename UComponent>
        [[nodiscard]] bool has() const {
            return m_entity_id.has<UComponent>();
        }

        //! @note Setting a component
        /**
            TODO: Try to allow the set component to be set to its initial value
                Initial value should be set when the component that is set is
           empty

        */
        template<typename UComponent>
        void set(const UComponent& p_component) {
            m_entity_id.set<UComponent>(p_component);
        }

        //! @note Set value for position and velocity component
        //! @note Flecs set component will be added if entity doesn't the
        //! component
        //! @note This is for setting multiple different components to be stored
        //! into the entity
        template<typename UComponent1, typename UComponent2>
        void set(const UComponent1& p_component1,
                 const UComponent2& p_component2) {
            //! @note When setting multiple components this is how flecs does it
            //! typically
            //! @note flecs::entity::set returns a const entity& to set multiple
            m_entity_id.set<UComponent1, UComponent2>(p_component1)
              .set(p_component2);
        }

        template<typename UComponent>
        void remove() {
            m_entity_id.remove<UComponent>();
        }

        operator flecs::entity() const { return m_entity_id; }
        
        operator flecs::entity() { return m_entity_id; }

    private:
        // Keep in mind that an entity just represent a 64-bit number
        flecs::entity m_entity_id;
    };
}; // namespace atlas