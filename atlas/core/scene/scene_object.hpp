#pragma once

#include <flecs.h>
#include <variant>
#include <tuple>

namespace atlas {
    class scene_object : public flecs::entity {
    public:
        scene_object() = default;
        scene_object(flecs::world_t* p_registry, flecs::entity_t p_id);

        scene_object(const flecs::entity& p_base);

        explicit scene_object(flecs::entity& p_base);

        void child_of(const scene_object& p_entity);

        /**
         * @brief Adds multiple components with no values assigned to them
         * EXPERIEMENTAL: Function to add multiple components but still in its
         * experiemental stages.
         *
         * Ideally this would be a shorthand for adding in multiple components
         * onto a single given entity
         *
         * Example:
         * ```C++
         * m_entity_example->add_query<atlas::transform, atlas::rigidbody>();
         * ```
         */
        template<typename... Args>
        void add_query() {
            using tuple_variadic = std::tuple<Args...>;
            std::variant<tuple_variadic> conditions;
            std::visit(
              [&](const auto& p_component) {
                  std::apply(
                    [&](auto&... p_placeholder) {
                        (add<std::decay_t<decltype(p_placeholder)>>(),
                         ...);
                    },
                    p_component);
              },
              conditions);
        }

        template<typename UFunction>
        void children(UFunction&& p_callback) {
            children(p_callback);
        }

        flecs::entity* operator->();
    };

    // class scene_object_exp {
    // public:
    //     scene_object_exp() = default;
    //     scene_object_exp(custom_entity& p_entity) : m_entity(&p_entity) {}


    //     custom_entity* operator->() const& {
    //         if(m_entity == nullptr) {
    //             throw std::runtime_error("Invalid access to game object!!");
    //         }

    //         return m_entity;
    //     }

    //     operator custom_entity*() {
    //         return m_entity;
    //     }

    // private:
    //     custom_entity* m_entity;
    // };
};