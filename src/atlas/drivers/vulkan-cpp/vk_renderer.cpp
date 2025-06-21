#include <drivers/vulkan-cpp/vk_renderer.hpp>
#include <core/engine_logger.hpp>
#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

#include <glm/glm.hpp>

namespace atlas::vk {

    vk_renderer::vk_renderer(const vk_swapchain& p_swapchain, const std::string& p_tag){
        console_log_manager::create_new_logger(p_tag);
        console_log_info("vk_renderer Begin construction!!!");
        m_main_swapchain = p_swapchain;
        m_image_count = p_swapchain.image_size();

        m_shader_group = vk_shader_group({
            {"experimental-shaders/test.vert", shader_stage::vertex},
            {"experimental-shaders/test.frag", shader_stage::fragment}
        });
        
        m_shader_group.set_vertex_attributes({
            { .location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex_input, position) },
            { .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex_input, color) },
            { .location = 2, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex_input, normals) },
            { .location = 3, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT,    .offset = offsetof(vk::vertex_input, uv) }
        });

        m_shader_group.set_vertex_bind_attributes({
            {.binding = 0, .stride = sizeof(vk::vertex_input), .inputRate = VK_VERTEX_INPUT_RATE_VERTEX}
        });

        console_log_info("vk_renderer End construction!!!");

        console_log_info("image_size = {}", m_image_count);

        m_global_uniforms = std::vector<vk_uniform_buffer>(p_swapchain.image_size());

        for(size_t i = 0; i < m_global_uniforms.size(); i++) {
            m_global_uniforms[i] = vk_uniform_buffer(sizeof(camera_ubo));
        }

        std::vector<VkDescriptorSetLayoutBinding> set0_descriptor_set_layout_bindings = {
            VkDescriptorSetLayoutBinding{.binding = 0, .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_VERTEX_BIT, .pImmutableSamplers  = nullptr},
            // VkDescriptorSetLayoutBinding{.binding = 1, .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, .pImmutableSamplers  = nullptr}
        };

        std::vector<VkDescriptorPoolSize> set0_allocation_info = {
            VkDescriptorPoolSize{
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = static_cast<uint32_t>(m_image_count),
            },
            // VkDescriptorPoolSize{
            //     .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            //     .descriptorCount = static_cast<uint32_t>(m_image_count),
            // }
        };


        /*
        struct descriptor_binding_point {
            uint32_t binding;
            shader_stage stage;
        };

        struct descriptor_binding_entry {
            buffer type;
            descriptor_binding_point bind_point;
            uint32_t descriptor_count;
        };

        // Usage: One per buffer_type in this case its only for one uniform at set = 0
        descriptor_binding_entry desc0_binding_entry = {
            .type = vk::buffer::uniform,
            .bind_point = {
                .binding = 0, equivalent to VkDescriptorSetLayoutBinding::binding
                .descriptor_count = 1, // equivalent to VkDescriptorSetLayoutBinding::descriptorCount
            },
            .allocation_count = m_image_count // allocation info for pool for VkDescriptorPoolSize::descriptorCount
        };

        std::array<descriptor_context> context = {desc0_context};

        descriptor_set_layout set0_layout = {
            .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
            .max_sets = m_image_count,                  // max of descriptor sets able to allocate
            .size_bytes = sizeof(camera_ubo),           // size of bytes of the uniforms utilized by this descriptor sets

            // This removes the need for .allocation_info and .bindings that takes in a std::span
            .entry = desc0_binding_entry                // Contains information related to the binding point, which takes in a std::span<descriptor_binding_entry>
        };
        
        */

        // Setting the parameters for setting up the descriptor set layout
        // Descriptor set = 0
        // descriptor_set_layout set0_descriptor_layout = {
        //     .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
        //     .max_sets = m_image_count,                  // max of descriptor sets able to allocate
        //     .size_bytes = sizeof(camera_ubo),           // size of bytes of the uniforms utilized by this descriptor sets
        //     .allocation_info = set0_allocation_info,         // specify the collection of multiple descriptor sets pool allocation sizes
        //     .bindings = set0_descriptor_set_layout_bindings  // specifying layout bindings specified in the shader for specific information thats gonna be passed into this descriptor sets
        // };
        std::vector<descriptor_binding_entry> entries = {
            descriptor_binding_entry{ // specifies "layout (set = 0, binding = 0) uniform GlobalUbo"
                .type = vk::buffer::uniform,
                .binding_point = {
                    .binding = 0,
                    .stage = shader_stage::vertex
                },
                .descriptor_count = 1,
            }
        };

        descriptor_set_layout set0_descriptor_layout = {
            .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
            .max_sets = m_image_count,                  // max of descriptor sets able to allocate
            .size_bytes = sizeof(camera_ubo),           // size of bytes of the uniforms utilized by this descriptor sets
            // .allocation_info = set0_allocation_info,         // specify the collection of multiple descriptor sets pool allocation sizes
            // .bindings = set0_descriptor_set_layout_bindings  // specifying layout bindings specified in the shader for specific information thats gonna be passed into this descriptor sets
            .entry = entries
        };

        // m_descriptor_set0 = descriptor_set(0, set0_descriptor_layout);
        m_descriptor_set0 = descriptor_set(0, set0_descriptor_layout);
        
        // Descriptor set = 1
        // Describing layout of descriptor set 1 that will be used across the meshes
        // std::vector<VkDescriptorSetLayoutBinding> set1_descriptor_set_layout_bindings = {
        //     VkDescriptorSetLayoutBinding{.binding = 0, .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_VERTEX_BIT, .pImmutableSamplers  = nullptr},
        //     VkDescriptorSetLayoutBinding{.binding = 1, .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, .pImmutableSamplers  = nullptr},
        //     VkDescriptorSetLayoutBinding{.binding = 2, .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, .pImmutableSamplers  = nullptr},
        // };

        // std::vector<VkDescriptorPoolSize> set1_allocation_info = {
        //     VkDescriptorPoolSize{
        //         .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        //         .descriptorCount = static_cast<uint32_t>(m_image_count),
        //     },
        //     VkDescriptorPoolSize{
        //         .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        //         .descriptorCount = static_cast<uint32_t>(m_image_count) * 2,
        //     },
        // };

        std::vector<descriptor_binding_entry> set1_entries = {
            descriptor_binding_entry{
                .type = vk::buffer::uniform,
                .binding_point = {
                    .binding = 0,
                    .stage = shader_stage::vertex
                },
                .descriptor_count = 1
            },
            descriptor_binding_entry{
                .type = vk::buffer::image_sampler,
                .binding_point = {
                    .binding = 1,
                    .stage = shader_stage::fragment
                },
                .descriptor_count = 1
            },
            descriptor_binding_entry{
                .type = vk::buffer::image_sampler,
                .binding_point = {
                    .binding = 2,
                    .stage = shader_stage::fragment
                },
                .descriptor_count = 1
            }
        };

        // descriptor_set_layout material_descriptor_layout = {
        //     .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
        //     .max_sets = m_image_count,                  // max of descriptor sets able to allocate
        //     .size_bytes = sizeof(material),           // size of bytes of the uniforms utilized by this descriptor sets
        //     .allocation_info = set1_allocation_info,         // specify the collection of multiple descriptor sets pool allocation sizes
        //     .bindings = set1_descriptor_set_layout_bindings  // specifying layout bindings specified in the shader for specific information thats gonna be passed into this descriptor sets
        // };
        descriptor_set_layout material_descriptor_layout = {
            .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
            .max_sets = m_image_count,                  // max of descriptor sets able to allocate
            .size_bytes = sizeof(material),           // size of bytes of the uniforms utilized by this descriptor sets
            .entry = set1_entries
        };
        m_descriptor_set1 = descriptor_set(1, material_descriptor_layout);

        m_descriptor_set2 = descriptor_set(1, material_descriptor_layout);

        m_mesh0_material_ubo = vk_uniform_buffer(sizeof(material));
        m_mesh1_material_ubo = vk_uniform_buffer(sizeof(material));

        std::vector<VkDescriptorSetLayout> set_layouts = {
            m_descriptor_set0.get_layout(),
            m_descriptor_set1.get_layout(),
            m_descriptor_set2.get_layout()
        };
        /*
            Pipeline Layout
            [Set 0] [Set 1] [Set 2]    [Set 3]
               |      |       |          |
               v      v       v          v
            Camera   Material Textures Lights
            UBO      UBO     UBO     UBO
        */
        m_main_pipeline = vk_pipeline(m_main_swapchain.swapchain_renderpass(), m_shader_group, set_layouts);


        /*

            Mesh 0 - Properties
                * Vbo, Ibo, Descriptor Set Layout
                * vk_uniform_buffer => uniform data layout mapped to shader call, the descriptor set will look for
        
        
        */

        /*
        To getting textures per object
        * Add textures and return std::span<texture>
        * Use this span to set our image_infos for VkImageView and VkSampler
        * Then once we do that we pass this into the set = 1, binding = 1 that is a sampler2D
        */
        m_mesh0 = mesh(std::filesystem::path("assets/models/viking_room.obj"));
        m_mesh0.add_texture(1, std::filesystem::path("assets/models/viking_room.png"));
        m_mesh0.add_texture(2, std::filesystem::path("assets/models/container_diffuse.png"));

        m_mesh1 = mesh(std::filesystem::path("assets/models/colored_cube.obj"));
        m_mesh1.add_texture(1, std::filesystem::path("assets/models/wood.png"));


        m_descriptor_set0.update(m_global_uniforms);
        // m_descriptor_set0.update(m_global_uniforms, {});
        console_log_error("textures.sizez() = {}", m_mesh0.read_textures().size());


        std::array<vk_uniform_buffer, 1> buffers = {
            m_mesh0_material_ubo
        };

        m_descriptor_set1.update(buffers, m_mesh0.read_textures());

        std::array<vk_uniform_buffer, 1> buffers2 = {
            m_mesh1_material_ubo
        };
        m_descriptor_set2.update(buffers2, m_mesh1.read_textures());

        vk_context::submit_resource_free([this](){
            m_shader_group.destroy();
            m_main_pipeline.destroy();
            // global uniforms camera data
            m_descriptor_set0.destroy();

            // mesh 0
            // mesh 0 - descriptor set for material
            // mesh 0 descriptor set - update with vk_uniform_buffer to write material uniforms
            m_descriptor_set1.destroy();

            // mesh 1
            // mesh 1 - descriptor set for material
            // mesh 1 descriptor set - update with vk_uniform_buffer to write material uniforms
            m_descriptor_set2.destroy();
            m_mesh0_material_ubo.destroy();
            m_mesh1_material_ubo.destroy();

            for(size_t i = 0; i < m_global_uniforms.size(); i++) {
                m_global_uniforms[i].destroy();
            }

            m_mesh0.destroy();
            m_mesh1.destroy();
        });
    }

    void vk_renderer::background_color(const std::array<float, 4>& p_color) {
        m_color = {{p_color.at(0), p_color.at(1), p_color.at(2), p_color.at(3)}};
    }

    void vk_renderer::start_frame(const vk_command_buffer& p_current, [[maybe_unused]] const vk::vk_swapchain& p_swapchain_handler) {
        // m_main_swapchain = p_swapchain_handler; // ?? This is here to do some testing with swapchain validation
        uint32_t current_frame = application::current_frame();

        std::array<VkClearValue, 2> clear_values = {};

        clear_values[0].color = m_color;
        clear_values[1].depthStencil = {1.f, 0};
        window_settings settings = m_main_swapchain.settings();

        VkRenderPassBeginInfo renderpass_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = m_main_swapchain.swapchain_renderpass(),
			.renderArea = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = {
					.width = settings.width,
					.height = settings.height
				},
			},
			.clearValueCount = static_cast<uint32_t>(clear_values.size()),
			.pClearValues = clear_values.data()
        };

        m_current_command_buffer = p_current;
        m_current_command_buffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        VkViewport viewport = {
            .x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(settings.width),
			.height = static_cast<float>(settings.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
        };

        vkCmdSetViewport(m_current_command_buffer, 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = {settings.width, settings.height},
		};

		vkCmdSetScissor(m_current_command_buffer, 0, 1, &scissor);

        renderpass_begin_info.framebuffer = m_main_swapchain.active_framebuffer(current_frame);

		vkCmdBeginRenderPass(m_current_command_buffer,&renderpass_begin_info,VK_SUBPASS_CONTENTS_INLINE);

        // camera uniforms
        static auto start_time = std::chrono::high_resolution_clock::now();

        auto current_time = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

        camera_ubo ubo{};
        ubo.Model = glm::mat4(1.f);
        ubo.Model = glm::translate(ubo.Model, glm::vec3(0.f, 0.f, -0.8f));
        ubo.Model = glm::scale(ubo.Model, glm::vec3(0.5f, 0.5f, 0.5f));
        ubo.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Projection = glm::perspective(glm::radians(45.0f), (float)settings.width / (float) settings.height, 0.1f, 10.0f);
        ubo.Projection[1][1] *= -1;

        // For now, using this. Will need to remove this before vulkan integration merging into dev
        // This is for testing and to hopefully have a global_ubo for globalized uniforms
        global_ubo global_frame_ubo = {
            .mvp = ubo.Projection * ubo.View
        };

        // glm::mat4 mvp = ubo.Projection * ubo.View * ubo.Model;
        m_global_uniforms[current_frame].update(&global_frame_ubo);
        material mesh0_material_ubo = {
            .model = ubo.Model,
            .color = {1.f, 1.f, 1.f, 1.0f}
        };
        m_mesh0_material_ubo.update(&mesh0_material_ubo);

        // Apply viking_room texture to mesh1 (viking_room.obj)
        m_main_pipeline.bind(p_current);
        m_descriptor_set0.bind(p_current, current_frame, m_main_pipeline.get_layout());
        m_descriptor_set1.bind(p_current, current_frame, m_main_pipeline.get_layout());

        m_mesh0.draw(p_current);

        // Drawing mesh2
        /*

        material_ubo is the ubo specific per-object
        * Meaning each scene object (mesh) is going to have their own descriptor set that is going to be for referencing the mesh material resource
        */
        ubo.Model = glm::mat4(1.f);
        ubo.Model = glm::translate(ubo.Model, glm::vec3(0.f, 0.f, -0.8f));
        ubo.Model = glm::scale(ubo.Model, glm::vec3(10.f, 10.f,10.f));
        ubo.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        material mesh1_material_ubo = {
            .model = ubo.Model,
            .color = {1.f, 1.f, 1.f, 0.5f}
        };
        
        m_mesh1_material_ubo.update(&mesh1_material_ubo);

        m_descriptor_set0.bind(p_current, current_frame, m_main_pipeline.get_layout());
        m_descriptor_set2.bind(p_current, current_frame, m_main_pipeline.get_layout());
        m_mesh1.draw(p_current);
        // Apply wood texture to mesh2 (sphere.obj)
        // m_main_pipeline.bind(p_current);
        // m_descriptor_set_test2.bind(p_current, current_frame, m_main_pipeline.get_layout());
        // m_test_mesh2.draw(p_current);

    }

    void vk_renderer::post_frame() {
        vkCmdEndRenderPass(m_current_command_buffer);
        m_current_command_buffer.end();

        m_main_swapchain.submit(m_current_command_buffer);
    }
};