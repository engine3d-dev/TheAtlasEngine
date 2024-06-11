#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Event/Event.h>
#include <engine3d/Core/Timestep.h>

namespace engine3d{

    // Layer interface
    // - Shouldn't have names (but only if its used for debugging)
    // TODO: allow enabling/disabling layers meaning not being updated or rendering (essentially hiding from layer stack)

    /**
     * @param OnAttach
     * @note Initiating layers
     * 
     * @param OnDetach
     * @note Deinitiating our laters (doing special cleanup)
     *
     * @param OnUpdate
     * @note Update our layers each frame
     * 
     * @param OnUIRender
     * @note Do our actual rendering, once the tasks are finished operating.
     */
    class ENGINE_API Layer{
    public:
        Layer(const std::string& name = "Layer No Name");
        virtual ~Layer();

        void OnAttach();
        void OnDetach();
        void OnEvent(Event& event);
        void OnUpdate(Timestep ts);
        void OnUIRender();

        std::string GetName() const;

    private:

        //! @note These are client-implementation functions to be handled by the client.
        //! @note When pushing onto the Layer Stack we attach it
        virtual void BeginPlay() = 0;

        //! @note Pop of thhe layer stack is for detaching (like shhutdown)
        virtual void EndPlay() = 0;

        //! @note Updating the layer is updated when the application calls onUpdate
        virtual void UpdateLayer(Timestep ts) = 0;

        // Updating any events being handled
        virtual void UpdateEvent(Event& event) = 0;

        //! @note Any UI-related rendering goes here
        virtual void UIRender() = 0;

    protected:
        std::string m_DebugName;
    };
};