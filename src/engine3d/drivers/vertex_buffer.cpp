#include <drivers/vertex_buffer.hpp>
#include <drivers/vertex_buffer.hpp>
#include <drivers/vulkan/vulkan_vertex_buffer.hpp>
#include <drivers/vulkan/vulkan_vertex_buffer.hpp>
#include <core/application_instance.hpp>
#include <glm/glm.hpp>

namespace engine3d{
    Ref<VertexBuffer> VertexBuffer::Create(const std::vector<Vertex>& p_Vertices){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanVertexBuffer>(p_Vertices);
        default: return nullptr;
        }

        return nullptr;
    }

    void VertexBuffer::Bind(VkCommandBuffer p_CommandBuffer){
        BindVertexBuffer(p_CommandBuffer);
    }

    void VertexBuffer::Draw(VkCommandBuffer p_CommandBuffer){
        RenderVertexBuffer(p_CommandBuffer);
    }

};