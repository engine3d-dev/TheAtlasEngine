import atlas;
#include <print>
#include <memory>
#include <optional>


class test_application : public atlas::application {
public:
    test_application(const atlas::application_settings& p_settings) : atlas::application(p_settings) {
    }
};


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

    // atlas::application_settings settings = {
    //     .name = "Editor",
    //     .width = 1900,
    //     .height = 600,
    //     .background_color = {1.f, 1.f, 1.f, 1.f},
    // };
    // atlas::ref<test_application> app = atlas::create_ref<test_application>(settings);

    atlas::ref<atlas::application> app = initialize_application();

    atlas::ref<atlas::graphics_context> context = atlas::initialize_context("vulkan", atlas::graphics_api::vulkan);

    app->execute();
    
    app->post_destroy();

    context->destroy();
    return 0;
}