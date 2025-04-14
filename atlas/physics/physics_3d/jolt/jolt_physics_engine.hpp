#include <physics/physics_3d/data/jolt_settings_data.hpp>
#include <physics/physics_3d/data/jolt_config_data.hpp>

namespace atlas::physics {

    /**
     * @brief Initializes physics global data for a particular scene
     *
     * @param physics_object An entity to allow the user to control the data
     * from the hierarchy. (within the editor)
     *
     */
    void create_engine(const jolt_settings_data& settings);

}