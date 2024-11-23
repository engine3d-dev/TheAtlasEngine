#pragma once
#include <cstdint>
#include <xhash>
#include <Core/Core.hpp>

namespace engine3d{
    /**
     * @name UUID
     * @note Traditionally a 128-bit value, as it is normally defined
     * @note Few options for generating these values
     * 
     * @note Generate UUID by construction
    */
    class UUID{
    public:
        UUID();
        UUID(uint64_t newID);
        UUID(const UUID& id) : uuid(id.uuid) {}

        //! @note Treat our UUID's as a uint64_t (at least returning our actual uuid)
        operator uint64_t() const { return uuid; }

    private:
        uint64_t uuid;
    };
};

namespace std{
    template<>
    struct hash<engine3d::UUID>{
        std::size_t operator()(const engine3d::UUID& uuid) const {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
};