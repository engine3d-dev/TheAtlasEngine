module;

#include <string>
#include <memory_resource>
#include <array>
#include <unordered_map>
#include <span>
#include <memory>

export module atlas.core.level_streamer;

import atlas.logger;
import atlas.core.scene;
import atlas.core.scene.uuid;
import atlas.core.event;


export namespace atlas {

    class level_streamer {
    public:

        /**
        * @brief Constructs a level streamer and allocates an arbitrary amount of bytes to the allocator
        */
        level_streamer(event::bus& p_bus) : m_bus(&p_bus) {
            std::array<uint8_t, 1024> bytes{};
            std::pmr::monotonic_buffer_resource resource_pool(bytes.data(), bytes.size());
            m_allocator.construct(&resource_pool);
        }

        ~level_streamer() {
            console_log_warn("~level_streamer called!!!!");
        }

        /**
        * @return the current scene
        */
        ref<scene> current_scene() {
            return nullptr;
        }

        /**
        * @brief Creating a new scene
        * 
        * Scenes are managed by the level streamer
        */
        void create_scene(const std::string& p_name) {
            uuid generate_id;
            console_log_info("Generated UUID: {}", static_cast<uint64_t>(generate_id));
            m_scenes[generate_id] = std::allocate_shared<scene>(m_allocator, p_name, *m_bus, generate_id);
        }

    private:
        std::pmr::polymorphic_allocator<scene> m_allocator;
        std::pmr::unordered_map<uint64_t, ref<scene>> m_scenes;
        event::bus* m_bus=nullptr;
    };

};