#pragma once
#include <unordered_map>
#include <cstdint>

namespace engine3d{
    template<typename T, typename... Rest>
    void HashCombine(size_t& seed, const T& v, const Rest&... rest){
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed << 2);
        (HashCombine(seed, rest), ...);
    }

};