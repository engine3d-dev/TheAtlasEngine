import atlas;
#include <print>
#include <memory>


class test_application : public atlas::application {
public:
    test_application(const atlas::application_settings& p_settings) : atlas::application(p_settings) {
        std::println("test_application initialized!!!");
        console_log_warn("test_application + logger initialized!!!");
    }
};


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

int main() {
    atlas::console_log_manager::initialize_logger_manager();
    atlas::ref<atlas::graphics_context> context = atlas::initialize_context(atlas::graphics_api::vulkan);
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1900,
        .height = 600,
        .background_color = {1.f, 1.f, 1.f, 1.f},
    };
    std::shared_ptr<test_application> app = std::make_shared<test_application>(settings);

    app->execute();
    
    app->post_destroy();
    return 0;
}