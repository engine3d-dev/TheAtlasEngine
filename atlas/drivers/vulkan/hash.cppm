module;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

export module atlas.drivers.vulkan.hash;

import vk;

export namespace atlas {
    template<typename T, typename... Rest>
    void hash_combine(size_t& seed, const T& v, const Rest&... rest) {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed << 2);
        (hash_combine(seed, rest), ...);
    }

};

export namespace std {

    // template<>
    // struct hash<atlas::vk::vertex_input> {
    //     size_t operator()(const atlas::vk::vertex_input& vertex) const {
    //         size_t seed = 0;
    //         atlas::hash_combine(
    //           seed, vertex.position, vertex.color, vertex.normals,
    //           vertex.uv);
    //         return seed;
    //     }
    // };
    template<>
    struct hash<vk::vertex_input> {
        size_t operator()(const vk::vertex_input& vertex) const {
            size_t seed = 0;
            atlas::hash_combine(
              seed, vertex.position, vertex.color, vertex.normals, vertex.uv);
            return seed;
        }
    };

}