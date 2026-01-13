
#include <GLFW/glfw3.h>
import atlas.application;
import atlas.common;
import atlas.logger;
import atlas.graphics_api;

import atlas.core.utilities.state;

// expose visibility to ref<graphics>
import atlas.drivers.graphics_context;

// this includes initialize_context
import atlas.drivers;
import atlas.core.scene.system_registry;

// Defined in the user-application side
[[nodiscard]] atlas::ref<atlas::application> initialize_application();


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