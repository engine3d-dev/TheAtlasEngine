#pragma once
#include <core/core.hpp>
#include <core/scene/world.hpp>
#include <core/event/event_bus.hpp>
#include <renderer/renderer.hpp>

class editor_world {
public:
    editor_world(const std::string& p_tag,
                 atlas::ref<atlas::renderer> p_renderer_instance);

private:
    atlas::ref<atlas::world> m_main_world;

    atlas::event::event_bus m_bus;
    atlas::ref<atlas::renderer> m_renderer;
};