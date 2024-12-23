#pragma once
#include <unordered_map>
#include <cstdint>
#include <glm/glm.hpp>

namespace engine3d{
    template <typename Vector3>
    concept IsVec3 = requires(Vector3 vector){
        vector.x;
        vector.y;
        vector.z;
    };

    template <typename Vector4>
    concept IsVec4 = IsVec3<Vector4> && requires(Vector4 vector){
        vector.w;
    };

    template <typename Vector3>
    concept IsGetterVec3 = requires(Vector3 vector){
        vector.GetX();
        vector.GetY();
        vector.GetZ();
    };

    template <typename Vector4>
    concept IsGetterVec4 = IsGetterVec3<Vector4> && requires(Vector4 vector){
        vector.GetW();
    };

    /*
    template<typename Vector3>
    constexpr glm::vec3 to_vec4(Vector3 p_Other){
        return {p_Other.GetX(), p_Other.GetY(), p_Other.GetZ()};
    }

    template<typename Vector4>
    constexpr glm::vec4 to_vec4(Vector4 p_Other){
        return {p_Other.GetX(), p_Other.GetY(), p_Other.GetZ(), p_Other.GetW()};
    }
    */

    template<typename T, typename... Rest>
    void HashCombine(size_t& seed, const T& v, const Rest&... rest){
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed << 2);
        (HashCombine(seed, rest), ...);
    }

};