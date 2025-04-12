#pragma once
#include <core/core.hpp>
#include <drivers/index_buffer.hpp>
#include <drivers/vertex_buffer.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace atlas {

    class mesh {
    public:
        mesh() = default;
        mesh(const std::string& p_filepath);
        mesh(const ref<vertex_buffer>& p_vertex_buffer,
             const ref<index_buffer>& p_index_buffer);
        [[nodiscard]] bool is_loaded() const;
        static mesh load(const std::string& p_filepath);
        // bool load_from_file(const std::string& p_filepath);

        ref<vertex_buffer>& get_vertex_buffer() { return m_vertex_buffer; }

        ref<index_buffer>& get_index_buffer() { return m_index_buffer; }

    private:
        ref<vertex_buffer> m_vertex_buffer;
        ref<index_buffer> m_index_buffer;
    };

}; // namespace atlas