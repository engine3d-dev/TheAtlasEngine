#include <core/system_framework/system_registry.hpp>
#include <core/scene/world.hpp>
#include <core/engine_logger.hpp>

namespace engine3d{
    // static SystemRegistry* s_GlobalSystemRegistry = nullptr;
    // WorldScope* SystemRegistry::s_CurrentWorld;
    // static WorldScope g_CurrentWorld;
    static WorldScope g_CurrentWorld;

    void SystemRegistry::InitializeRegistry(){
        // s_CurrentWorld = nullptr;
    }

    // void SystemRegistry::Register(WorldScope* p_World){
    void SystemRegistry::Register(WorldScope* p_World){
        g_CurrentWorld = *p_World;

        // ConsoleLogInfo("Register::p_World->GetTag() = {}", p_World->GetTag());
        ConsoleLogInfo("Register::p_World->GetTag() = {}", p_World->GetTag());
        ConsoleLogInfo("Register::g_CurrentWorld->GetTag() = {}", g_CurrentWorld.GetTag());
    }

    // WorldScope* SystemRegistry::GetWorld(){
    WorldScope SystemRegistry::GetWorld(){
        ConsoleLogInfo("g_CurrentWorld->GetTag() = {}", g_CurrentWorld.GetTag());
        // return &g_CurrentWorld;
        return g_CurrentWorld;
    }
};