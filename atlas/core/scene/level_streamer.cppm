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
        * @return the currently active scene to retrieve
        */
        ref<scene> current_scene(const std::string& p_name) {
            return m_scenes[p_name];
        }

        template<typename UScene>
        void default_scene(const std::string& p_name, event::bus& p_bus) {
            m_scenes.emplace(p_name, std::allocate_shared<UScene>(std::pmr::polymorphic_allocator<UScene>(m_allocator.resource()), p_name, p_bus));
        }



        /**
        * @brief used to iterate through over the scenes created
        *
        * Example Usage:
        * ```C++
        * m_level_streamer.each([this](ref<scene> p_scene){
        *       // do some logic
        * });
        * ```
        * 
        */
        template<typename UCallback>
        void each(UCallback&& p_callback) {
            for(auto[uuid, scene] : m_scenes) {
                p_callback(scene);
            }
        }

    private:
        std::pmr::polymorphic_allocator<> m_allocator;
        std::pmr::unordered_map<std::string, ref<scene>> m_scenes;
        event::bus* m_bus=nullptr;
    };

};