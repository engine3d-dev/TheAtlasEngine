// import atlas;
#include <print>
#include <memory>
#include <optional>
#include <GLFW/glfw3.h>

import atlas.application;
import atlas.common;
import atlas.logger;
import atlas.graphics_api;

import atlas.core.utilities.state;

import atlas.drivers.graphics_context;
import atlas.drivers;
import atlas.core.event;
import atlas.core.event.keys;
import editor_world;
import atlas.core.scene.system_registry;

class test_application : public atlas::application {
public:
    test_application(const atlas::application_settings& p_settings) : atlas::application(p_settings) {
        // atlas::register_update(this, &test_application::on_update);std::pmr::monotonic_buffer_resource resource{ 4096 };
        // m_allocator.construct(&resource);

        // TODO -- this is going to be changed with the use of the level
        // streamer API
        console_log_info("renderer_instance = {}", (renderer_instance() != nullptr));
        m_world = atlas::create_ref<editor_world>("Editor World", renderer_instance());
    }


    void on_update() {
        if(atlas::event::is_key_pressed(key_escape)) {
            atlas::application::destroy();
        }
    }


private:
    // std::pmr::polymorphic_allocator<uint8_t> m_allocator;
    atlas::ref<editor_world> m_world;

};


/*

TODO: Pre-defined this in TheAtlasEngine
* Then to have the implemented by the application

*/
atlas::ref<atlas::application> initialize_application() {
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1900,
        .height = 600,
        .background_color = {1.f, 1.f, 1.f, 1.f},
    };
    return create_ref<test_application>(settings);
}


// /*
// atlas/*
//     core/*
//         import atlas.core;
//     drivers/
//         vulkan
//             import atlas.vulkan
//         jolt_cpp
//             import atlas.jolt_cpp
//         ....
//     physics
//         import atlas.physics
//     renderer
//         import atlas.renderer
// */

// /*
// core: module that is partitioned associated with
//     logger
//     import logger;
// drivers:
//     vulkan: module that is partitioned associated with
//         import vulkan;
//             internal: export module vulkan:*
//     jolt_cpp: module that is partitioned associated with
//         import jolt_cpp
//             internal: export module jolt_cpp:*
// atlas: global module unit
//     export import core;
//     export import vulkan;
//     export import jolt_cpp
// */

int main() {
    atlas::console_log_manager manager = atlas::console_log_manager();

    if(!glfwInit()) {
        console_log_fatal("GLFW: Initialization failed!!");
        return -1;
    }

    // atlas::application_settings settings = {
    //     .name = "Editor",
    //     .width = 1900,
    //     .height = 600,
    //     .background_color = {1.f, 1.f, 1.f, 1.f},
    // };
    // atlas::ref<test_application> app = atlas::create_ref<test_application>(settings);

    atlas::ref<atlas::system_registry> system = atlas::create_ref<atlas::system_registry>("system");
    atlas::ref<atlas::graphics_context> context = atlas::initialize_context("vulkan", atlas::graphics_api::vulkan);

    atlas::ref<atlas::application> app = initialize_application();

    app->execute();
    
    app->post_destroy();

    context->destroy();
    return 0;
}