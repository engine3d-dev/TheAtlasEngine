#pragma once
#include <glm/fwd.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace engine3d::vk{
    /**
     * @name VulkanModel
     * @note For now I am going to have each model contain vertices, indices, and so forth.
     * @note And may separate the vertex/index/storage buffers as their own thing but for the time being, just gonna have them be under this one class
    */
    class VulkanModel{
    public:
        //! @note Can be the way we structure what data types is accepted in our shader.
        struct Vertex{
            glm::vec3 Position;
            glm::vec3 Color;
            
            static std::vector<VkVertexInputBindingDescription> GetVertexInputBindDescription();
            static std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();
        };

        VulkanModel() = default;
        VulkanModel(const std::vector<Vertex>& p_Vertices, const VkBufferUsageFlags& p_UsageFlags, const VkMemoryPropertyFlags& p_PropertyFlags);


        void Bind(VkCommandBuffer p_Command);
        void Draw(VkCommandBuffer p_Command);

        static std::vector<Vertex>* CreateVertexVector(std::vector<glm::vec3> p_Vertices, std::vector<glm::vec3> p_Colors);
        static std::vector<Vertex>* CreateVertexVector(std::vector<glm::vec3> p_Vertices);
        
    private:
        void initialize_vertex_buffers(const std::vector<Vertex>& p_Vertices);

    private:
        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferDeviceMemory;
        uint32_t m_VerticesCount;
    };
};