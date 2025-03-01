#pragma once
#include <glm/glm.hpp>
#include <core/core.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace atlas{
    /*
        @name vertex_buffer
        @note Contains vertices and does backend API stuff.
    */
    struct vertex{
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec3 Normals;
        glm::vec2 TexCoords;

        bool operator==(const vertex& other) const{
            return (Position == other.Position and Color == other.Color and Normals == other.Normals and TexCoords == other.TexCoords);
        }
    };

    class vertex_buffer{
    public:
        virtual ~vertex_buffer() = default;

        static ref<vertex_buffer> create(const std::vector<vertex>& p_vertices);

        void bind(const VkCommandBuffer& p_command_buffer);

        void draw(const VkCommandBuffer& p_command_buffer);

    protected:
        virtual void bind_vertex_buffer(const VkCommandBuffer& p_command_buffer) = 0;
        virtual void render_vertex_buffer(const VkCommandBuffer& p_command_buffer) = 0;
    };
};