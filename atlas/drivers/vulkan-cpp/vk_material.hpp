#pragma once
#include <vulkan/vulkan.h>
#include <span>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_texture.hpp>
#include <drivers/vulkan-cpp/vk_descriptor_set.hpp>

namespace atlas::vk {
    /**
     * @brief vulkan implementation of a material class
     * 
     * Contains all the specifications for a material source neede to communicate through vulkan to the shader.
     * 
     * Material specifications that vulkan required is specified in this material_resource class
     * 
     * @brief Represents a single material source to define how a scene object may get rendered
     * 
     * 
     * API 
     * albedo_layout being the descriptor set layout itself
     * material_resource albedo_material = material_resource("albedo", albedo_layout, std::filesystem::path("albedo_image.png"));
    */
    class material_source {
    public:
        material_source() = default;

        //! @brief Passes in the descriptor set layout utilized by the set material
        material_source(const std::string& p_name, const std::span<descriptor_set_layout>& p_layouts, const std::filesystem::path& p_texture_filepath);

        //! @note Returns descriptor set with this material layout
        [[nodiscard]] descriptor_set set() const { return m_material_descriptor_set; }

        //! @brief Returns name this material is associated with
        [[nodiscard]] std::string name() const { return m_name; }

    private:
        std::string m_name="";
        descriptor_set m_material_descriptor_set{};
    };
};