#pragma once
// #include <entt/entt.hpp>

namespace engine3d{
    /**
     * @name Registry
     * @note Handler and manager for the registry specifically
     * @note Because entt::registry is what manages entities and it's components
     * @note This wrapper will provide a higher-level API in which how scene object's components get managed
    */
    // class Registry : public entt::registry{
    // public:
    //     Registry() = default;

    //     ~Registry(){
    //         Clear();
    //     }

    //     template<typename UComponent, typename... Args>
    //     void OnComponentAdd(entt::entity p_EntityID, Args&&... args){
    //         m_RegistryContainer.emplace<UComponent>(p_EntityID, std::forward<Args>(args)...);
    //     }

    //     template<typename UComponent>
    //     UComponent& GetComponent(entt::entity p_EntityID){
    //         return m_RegistryContainer.get<UComponent>(p_EntityID);
    //     }

    //     template<typename UComponent>
    //     bool HasComponent(entt::entity p_EntityID){
    //         return m_RegistryContainer.all_of<UComponent>(p_EntityID);
    //     }

    //     entt::entity CreateEntity(){
    //         return entt::entity{m_RegistryContainer.create()};
    //     }

    //     void DestroyEntity(entt::entity& p_EntityID){
    //         m_RegistryContainer.destroy(p_EntityID);
    //     }

    //     void Clear(){
    //         m_RegistryContainer.clear();
    //     }

    // private:
    //     entt::registry m_RegistryContainer;
    // };
};