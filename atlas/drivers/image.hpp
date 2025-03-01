#pragma once
#include <string>


/*

What we could do is for world and scenes

Is having a hash tag that is associated to the world.
- This can be used to allow users not have to worry about selecting which world they want to register_to their scene to
- system_registry::get_world()->register_to(this)
- Where get_world() fetches the world_hashID where this can be stored in a bitset
    - Where we can probably set the first 8-bits in an integer to the hash is the ID of the world the scene is associated with
    - While the last 8-bits tell what the actual scene's hash ID itself
    - These hashes are something added to the scene when they get added to the world
    - When the world is constructed, the system_registry will associate a unique hash to that world
*/

namespace atlas{
    struct ImageSampler{
    };

    class Image{
    public:
        Image(const std::string& p_filepath);
    private:
    };
};