#include <Core/ApplicationManager/GameObjManager/UUID.hpp>
#include <random>

namespace Engine3D
{
    static std::random_device randomDevice;
    static std::mt19937_64 engine(randomDevice());
    static std::uniform_int_distribution<uint64_t> uniformDistribution;

    UUID::UUID() : uuid(uniformDistribution(engine)) {
    }

    UUID::UUID(uint64_t newID){}
};