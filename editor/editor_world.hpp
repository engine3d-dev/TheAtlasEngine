#pragma once
#include <core/scene/world.hpp>
#include <core/event/event_bus.hpp>

class editor_world {
public:
    editor_world();
    editor_world(const std::string& p_tag);

private:
    atlas::ref<atlas::world> m_main_world;

    atlas::event::event_bus m_bus;
};