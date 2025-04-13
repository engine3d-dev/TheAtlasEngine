#include <core/serialize/serializer.hpp>
#include <flecs/addons/cpp/entity.hpp>
#include <fstream>
#include <glm/fwd.hpp>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/yaml.h>

namespace YAML {
    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);

            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();

            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);

            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();

            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {

        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);

            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
};

namespace atlas {
    // static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v){
    // 	out << YAML::Flow;
    // 	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    // 	return out;
    // }

    /**
     * NativeScriptComponent
     * - Allows to bind user-native scripts to be binded to this component
     * - Allows for entities to do:
     * PropellerController propeller_data = { // do stuff... };
     *
     * m_SceneObject->add<NativeScriptComponent>(&propeller_data);
     * const PropellerComponent*
     * static_cast<Data>(m_SceneObject->get<NativeScriptComponent>()->Data);
     *
     */

    static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {

        // m_SceneObject->add<NativeScriptComponent>(&propeller_data);
        // const PropellerComponent* =
        // static_cast<PropellerController>(m_SceneObject->get<NativeScriptComponent>()->Data);

        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    // actual serialization with yaml-cpp
    [[maybe_unused]] static void serialize_entity(YAML::Emitter& output,
                                 const flecs::entity& p_entity) {
        output << YAML::BeginMap;

        output << YAML::Key << "Entity" << YAML::Value
               << p_entity.get<tag>()->TagMetadata;

        if (p_entity.has<transform>()) {
            output << YAML::Key << "Transform";

            output << YAML::BeginMap;
            auto entity_transform = p_entity.get<transform>();
            output << YAML::Key << "Position" << YAML::Value
                   << entity_transform->Position;
            output << YAML::Key << "Scale" << YAML::Value
                   << entity_transform->Scale;
            output << YAML::Key << "Rotation" << YAML::Value
                   << entity_transform->Rotation;
            output << YAML::Key << "Color" << YAML::Value
                   << entity_transform->Color;
            output << YAML::EndMap;
        }

        if (p_entity.has<camera>()) {
            output << YAML::Key << "Perspective Camera";
            // output << YAML::Key << "Camera" << YAML::Value;

            auto perspective_camera = p_entity.get<camera>();

            output << YAML::BeginMap;
            output << YAML::Key << "Position" << YAML::Value
                   << perspective_camera->Position;
            // output << YAML::Key << "Front" << YAML::Value
            //        << perspective_camera->Front;
            output << YAML::Key << "Up" << YAML::Value
                   << perspective_camera->Up;
            output << YAML::Key << "Down" << YAML::Value
                   << perspective_camera->Down;
            output << YAML::Key << "Right" << YAML::Value
                   << perspective_camera->Right;
            output << YAML::Key << "Left" << YAML::Value
                   << perspective_camera->Left;
            output << YAML::Key << "Euler"
                   << YAML::Value
                   //    << perspective_camera->EulerRotation;
                   << perspective_camera->EulerRotation;
            output << YAML::Key << "MovementSpeed" << YAML::Value
                   << perspective_camera->MovementSpeed;
            output << YAML::Key << "MouseSensitivity" << YAML::Value
                   << perspective_camera->MouseSensitivity;
            output << YAML::Key << "Zoom" << YAML::Value
                   << perspective_camera->Zoom;

            output << YAML::EndMap;
        }

        if (p_entity.has<rendertarget3d>()) {
            output << YAML::Key << "Mesh Component";
            // output << YAML::Key << "Mesh" << YAML::Value;

            auto mesh_component = p_entity.get<rendertarget3d>();

            output << YAML::BeginMap;

            output << YAML::Key << "Filepath" << YAML::Value
                   << mesh_component->Filepath;

            output << YAML::EndMap;
        }

        output << YAML::EndMap; // Entity
    }

    serializer::serializer([[maybe_unused]] const scene_scope* p_scene_ctx) {
        m_current_scene_ctx = create_ref<scene_scope>(*p_scene_ctx);
    }

    void serializer::save_as(const std::string& p_filepath) {
        YAML::Emitter output;
        output << YAML::BeginMap;
        output << YAML::Key << "Scene" << YAML::Value
               << m_current_scene_ctx->get_tag();
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        // flecs::world* world_object = atlas::system_registry::get_world().get_registry();
        // ref<world_scope> world_object = system_registry::get_world("Editor World");
        // ref<scene_scope> current_scene = world_object->get_scene("LevelScene");
        // flecs::query<> q = current_scene->query_builder<atlas::tag>().build();
        // flecs::world world_registry = world_object->

        //! @note Queries in flecs the ecs framework are how we can query all
        //! entities that the engine (user creates through our API)
        // flecs::query<> q =
        //   world_object->query_builder().with<atlas::tag>().build();

        // q.each([&output](flecs::entity p_entity_id) {
        //     serialize_entity(output, p_entity_id);
        // });

        std::ofstream output_file(p_filepath);
        output_file << output.c_str();
    }
};