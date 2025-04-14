#include <core/scene/components.hpp>
#include <core/scene/entity.hpp>

namespace atlas {
    entity_t::entity_t(flecs::world* p_registry, const std::string& p_tag) {
        m_entity_id = p_registry->entity(p_tag.c_str());

        //! @note These are default components the entity will store that all
        //! game objects SHOULD have by default.
        add<transform>();
        set<tag>({ p_tag });
    }

    entity_t::~entity_t() {
        //! @note We make sure that if this entity is alive we can destruct it
        //! @note Destructor will only be called if the object goes out of scope
        if (this->is_alive()) {
            on_destruction();
        }
    }
};