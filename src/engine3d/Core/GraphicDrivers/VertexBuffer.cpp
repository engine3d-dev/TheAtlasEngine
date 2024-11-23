#include <Core/GraphicDrivers/VertexBuffer.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanVertexBuffer.hpp>
#include <Core/ApplicationInstance.hpp>
#include <glm/glm.hpp>

namespace engine3d{
    Ref<VertexBuffer> VertexBuffer::Create(const std::vector<Vertex>& p_Vertices){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanVertexBuffer>(p_Vertices);
        default: return nullptr;
        }

        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::CreateVertexVector(const std::vector<glm::vec3>& p_Vertices, std::vector<glm::vec3> p_Colors){
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::CreateVertexVector(const std::vector<glm::vec3>& p_Vertices){
        return nullptr;
    }

    void VertexBuffer::Bind(VkCommandBuffer p_CommandBuffer){
        BindVertexBuffer(p_CommandBuffer);
    }

    void VertexBuffer::Draw(VkCommandBuffer p_CommandBuffer){
        RenderVertexBuffer(p_CommandBuffer);
    }

};