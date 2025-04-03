#include <core/scene/scene_object.hpp>

namespace atlas::physics {

    /**
     * @brief Initializes physics global data for a particular scene
     * 
     * @param physics_object An entity to allow the user to control the data
     * from the hierarchy. (within the editor)
     * 
     */
    void create(ref<atlas::scene_object> physics_object);
    
}