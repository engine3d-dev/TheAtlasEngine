#pragma once
#include <core/core.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace atlas{
    class index_buffer{
    public:
        virtual ~index_buffer() = default;
        static ref<index_buffer> create(const std::vector<uint32_t>& p_indices);

        void bind(const VkCommandBuffer& p_command_buffer);
        void draw(const VkCommandBuffer& p_command_buffer);

        bool has_indices() const;

    protected:
        virtual void bind_to_index_buffer(const VkCommandBuffer& p_command_buffer) = 0;
        virtual void render_index_buffer(const VkCommandBuffer& p_command_buffer) = 0;
        virtual bool contains_indices() const = 0;
    };
};