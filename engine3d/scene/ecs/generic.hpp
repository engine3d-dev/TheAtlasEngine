#pragma once
#include <flecs.h>
#include <string>

namespace engine3d{
    /**
     * @file generic.hpp
     * @note This file is for handling generic templated functions the ecs uses
    */
    template<typename T, typename U, typename Tag>
    void System(flecs::world* p_Registry, const std::string& p_Tag, const Tag& p_FlecsPipelineTag){
        p_Registry->system<T, U>(p_Tag.c_str()).kind(p_FlecsPipelineTag);
    }
};