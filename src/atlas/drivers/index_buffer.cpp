#include <core/application.hpp>
#include <drivers/vulkan/vulkan_index_buffer.hpp>

namespace atlas {
    ref<index_buffer> index_buffer::create(
      const std::vector<uint32_t>& p_indices) {
        switch (application::current_api()) {
            case API::VULKAN:
                return create_ref<vk::vk_index_buffer>(p_indices);
            default:
                return nullptr;
        }

        return nullptr;
    }

    void index_buffer::bind(const VkCommandBuffer& p_command_buffer) {
        return bind_to_index_buffer(p_command_buffer);
    }

    void index_buffer::draw(const VkCommandBuffer& p_command_buffer) {
        return render_index_buffer(p_command_buffer);
    }

    bool index_buffer::has_indices() const {
        return contains_indices();
    }
};