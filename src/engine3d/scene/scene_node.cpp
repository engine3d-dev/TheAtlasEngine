#include <scene/scene_object.hpp>
#include <scene/components/components.hpp>

namespace engine3d{
    SceneNode::SceneNode(Registry& p_Registry, const std::string& Tag) : m_Tag(Tag) {
        m_ObjectEntity = p_Registry.CreateEntity();
        AddComponent<Transform>();
    }
};