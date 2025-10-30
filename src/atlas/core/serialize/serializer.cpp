#include <core/serialize/serializer.hpp>
#include <core/scene/components.hpp>
#include <fstream>
#include <yaml-cpp/emitter.h>
#include <core/system/registry.hpp>
#include <core/serialize/types.hpp>
#include <physics/components.hpp>

namespace atlas {

    // used to serialize entities
    // TODO -- expand on this to stream_reader and stream_writer
    static void serialize_entity(YAML::Emitter& output,
                                 const flecs::entity& p_entity) {
        output << YAML::BeginMap;

        output << YAML::Key << "Entity" << YAML::Value << p_entity.name();

        if (p_entity.has<transform>()) {
            output << p_entity.get<transform>();
        }

        if (p_entity.has<perspective_camera>()) {
            output << p_entity.get<perspective_camera>();
        }

        if (p_entity.has<mesh_source>()) {
            output << p_entity.get<mesh_source>();
        }

        if (p_entity.has<physics_body>()) {
            output << p_entity.get<physics_body>();
        }

        if (p_entity.has<box_collider>()) {
            output << p_entity.get<box_collider>();
        }

        if (p_entity.has<sphere_collider>()) {
            output << p_entity.get<sphere_collider>();
        }

        if (p_entity.has<capsule_collider>()) {
            output << p_entity.get<capsule_collider>();
        }

        // serialize entity childrens -- TODO
        // output << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
        // p_entity.children([&](flecs::entity p_child_entity){
        // 	serialize_entity(output, p_child_entity);
        // });

        output << YAML::EndMap;
    }

    static void deserialize_entity(YAML::iterator::value_type p_entity_value,
                                   flecs::entity& p_deserialize_to_object) {
        if (p_entity_value["Transform"]) {
            auto transform_data = p_entity_value["Transform"];
            p_deserialize_to_object.set<transform>({
              .position = transform_data["Position"].as<glm::vec3>(),
              .quaternion = transform_data["Quaternion"].as<glm::highp_vec4>(),
              .rotation = transform_data["Rotation"].as<glm::vec3>(),
              .scale = transform_data["Scale"].as<glm::vec3>(),
            });
        }

        // Deserialize atlas::perspective_camera component
        if (p_entity_value["PerspectiveCamera"]) {
            auto perspective_camera_data = p_entity_value["PerspectiveCamera"];
            p_deserialize_to_object.set<perspective_camera>({
              .plane = perspective_camera_data["Plane"].as<glm::vec2>(),
              .is_active = perspective_camera_data["Active"].as<bool>(),
              .field_of_view =
                perspective_camera_data["Field of View"].as<float>(),
            });
        }

        // Deserialize atlas::material component
        if (p_entity_value["Mesh Source"]) {
            auto perspective_camera_data = p_entity_value["Mesh Source"];
            p_deserialize_to_object.set<mesh_source>({
              .model_path =
                perspective_camera_data["Model Path"].as<std::string>(),
              .texture_path =
                perspective_camera_data["Texture Path"].as<std::string>(),
            });
        }

        // deserialize physics body
        if (p_entity_value["Physics Body"]) {
            auto body = p_entity_value["Physics Body"];
            p_deserialize_to_object.set<physics_body>(
              { .linear_velocity = body["Linear Velocity"].as<glm::vec3>(),
                .angular_velocity = body["Angular Velocity"].as<glm::vec3>(),
                .force = body["Force"].as<glm::vec3>(),
                .impulse = body["Impulse"].as<glm::vec3>(),
                .torque = body["Torque"].as<glm::vec3>(),
                .center_mass_position =
                  body["Center Mass Position"].as<glm::vec3>(),
                .friction = body["Friction"].as<float>(),
                .restitution = body["Restitution"].as<float>(),
                .body_movement_type =
                  static_cast<body_type>(body["Body Movement Type"].as<int>()),
                .body_layer_type =
                  static_cast<body_layer>(body["Body Layer Type"].as<int>()) });
        }

        if (p_entity_value["Box Collider"]) {
            auto collider = p_entity_value["Box Collider"];
            p_deserialize_to_object.set<box_collider>({
              .half_extent = collider["Half Extent"].as<glm::vec3>(),
            });
        }

        if (p_entity_value["Sphere Collider"]) {
            auto collider = p_entity_value["Sphere Collider"];
            p_deserialize_to_object.set<sphere_collider>({
              .radius = collider["Radius"].as<float>(),
            });
        }

        if (p_entity_value["Capsule Collider"]) {
            auto collider = p_entity_value["Capsule Collider"];
            p_deserialize_to_object.set<capsule_collider>({
              .radius = collider["Half Height"].as<float>(),
              .half_height = collider["Half Height"].as<float>(),
            });
        }
    }

    serializer::serializer(const ref<scene_scope>& p_scene_ctx)
      : m_current_scene_ctx(p_scene_ctx) {}

    void serializer::save(const std::filesystem::path& p_filepath) {
        YAML::Emitter output;
        output << YAML::BeginMap;
        output << YAML::Key << "Scene" << YAML::Value
               << m_current_scene_ctx->name();
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        //! @note Queries in flecs the ecs framework are how we can query all
        //! entities that the engine (user creates through our API)
        // ref<world_scope> world_object =
        //   system_registry::get_world("Editor World");
        // ref<scene_scope> current_scene =
        // world_object->get_scene("LevelScene");

        // flecs::query<> q =
        //   current_scene->query_builder().with<atlas::transform>().build();

        // query all entities with a serialized tag specified
        // while specifying to not query entities that also have the tag::editor
        // specified
        flecs::query<> q = m_current_scene_ctx->query_builder()
                             .with<tag::serialize>()
                             .without<tag::editor>()
                             .build();

        q.each([&output](flecs::entity p_entity_id) {
            serialize_entity(output, p_entity_id);
        });

        std::ofstream output_file(p_filepath.string());
        output_file << output.c_str();
    }

    bool serializer::load(const std::filesystem::path& p_filepath,
                          const flecs::world& p_registry) {
        std::ifstream ins(p_filepath.string());
        std::stringstream ss;
        ss << ins.rdbuf();

        YAML::Node data = YAML::Load(ss.str());

        if (!data["Scene"]) {
            return false;
        }

        YAML::Node entity_objects = data["Entities"];

        if (entity_objects) {
            for (YAML::iterator::value_type entity : entity_objects) {
                std::string name = entity["Entity"].as<std::string>();
                flecs::entity deserialize_to_object =
                  p_registry.entity(name.c_str());

                // Deserialize atlas::transform component
                deserialize_entity(entity, deserialize_to_object);
            }
        }

        return true;
    }
};