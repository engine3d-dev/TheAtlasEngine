#include <engine3d/Core/core.h>
#include <engine3d/Core/Layer.h>


namespace Engine3D{

    /**
     * 
     * @name UILayer
     * @note Our base layer for UI-event based tasks
     * @note Our UI Editor will be applied to this layer
     * @note This layer, is going to be handling all of our UI-related stuff
     * @note TODO for today --- Get our viewport working with ImGUi
     * @note Prevent from our mouse to work when we are no longer hovering our viewport
    */
    class UILayer : public Layer{
    private:
        void Attach() override;
        void Detach() override;

        void UpdateLayer(Timestep ts) override;
        void UpdateEvent(Event& event) override;

        void UIRender() override;

    private:
        // void InitializeDockspace();

    private:
        bool m_IsViewportHovered, m_IsViewportFocused;
    };
};