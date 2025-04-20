// // #include <physics/physics_3d/physics_engine_backend.hpp>
// #include <core/scene/scene_object.hpp>


// namespace atlas::physics {

//     enum physics_api : uint8_t {
//         Jolt
//     };

//     physics_api test_api = Jolt;

//     ref<physics_engine_backend> initialize_engine(const physics_settings& p_settings) {
//         switch(test_api){
//              case physics_api::Jolt: return create_ref<jolt_engine>(p_settings);l
//              default: return nullptr;
//         }
//         return nullptr;
//     }

//     void create(const ref<scene_object>& p_physics_object) {

//         if (p_physics_object == nullptr) {
//             console_log_fatal("Missing physics scene object.");
//             return;
//         }

//         if (p_physics_object->get<physics_settings>() == nullptr) {
//             console_log_fatal("Missing jolt_settings component.");
//             return;
//         }

//         engine->start_engine(*p_physics_object->get<physics_settings>());
//     }

    
// }