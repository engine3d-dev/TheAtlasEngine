module;

// #include <core/core.hpp>
// #include <core/scene/scene.hpp>
#include <string>
#include <filesystem>
#include <flecs.h>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <glm/glm.hpp>
#include <fstream>

export module atlas.core.serialize;

import atlas.common;
import atlas.core.scene;
import atlas.core.serialize.types;
import atlas.core.scene.components;


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

        if (p_entity.has<point_light>()) {
            output << p_entity.get<point_light>();
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
              .diffuse = perspective_camera_data["Diffuse"].as<std::string>(),
              .specular = perspective_camera_data["Specular"].as<std::string>(),
            });
        }

        if (p_entity_value["Point Light"]) {
            auto point_light = p_entity_value["Point Light"];
            p_deserialize_to_object.set<atlas::point_light>({
              .color = point_light["Color"].as<glm::vec4>(),
              .attenuation = point_light["Attenuation"].as<float>(),
              .constant = 1.f,
              .linear = 1.f,
              .quadratic = 1.f,
              .ambient = point_light["Ambient"].as<glm::vec4>(),
              .diffuse = point_light["Diffuse"].as<glm::vec4>(),
              .specular = point_light["Specular"].as<glm::vec4>(),
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

    /**
     * @brief serializer is responsible for saving/loading scenes
     *
     * This class excepts a scene context with the purpose of serializing with
     * the entities that reside within that specific scene context
     *
     * Which also provide an API for loading in a scene from disk for
     * deserialization.
     */
    export class serializer {
    public:
        serializer() = default;

        /**
         * @brief constructs a new serializer with a requirement to specify a
         * scene to serialize
         * @param p_scene_ctx is the current scene to perform
         * serialization/deserialization to
         */
        serializer(const ref<scene>& p_scene_ctx) : m_current_scene_ctx(p_scene_ctx) {}

        /**
         * @param p_filepath is the specified path to save the file
         */
        void save(const std::filesystem::path& p_filepath) {
            YAML::Emitter output;
            output << YAML::BeginMap;
            output << YAML::Key << "Scene" << YAML::Value
                << m_current_scene_ctx->name();
            output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

            //! @note Queries in flecs the ecs framework are how we can query all
            //! entities that the engine (user creates through our API)
            // ref<world_scope> world_object =
            //   system_registry::get_world("Editor World");
            // ref<scene> current_scene =
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

        /**
         * @param p_filepath is the specified path to loading in the saved file
         * from
         * @param p_registry is the current scene registry to load/create the
         * entities through that registry
         * @return true if loading was successful, otherwise will return false
         */
        bool load(const std::filesystem::path& p_filepath, const flecs::world& p_registry) {
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

    private:
        ref<scene> m_current_scene_ctx;
    };

}; // namespace atlas