#pragma once
#include <drivers/vulkan-cpp/vk_types.hpp>

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/hash.hpp>

// namespace std {

//     /**
//      * @brief Specialization for hash function for using vertex into the hash table
//     */
//     template<>
//     struct hash<atlas::vk::vertex> {
//         size_t operator()(atlas::vk::vertex const& vertex) const {
//             return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec3>()(vertex.normals) << 1)  ^ (hash<glm::vec2>()(vertex.uv) << 1);
//         }
//     };
// }

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
    template<>
    struct hash<atlas::vk::vertex> {
        size_t operator()(atlas::vk::vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec3>()(vertex.normals) << 1)  ^ (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}