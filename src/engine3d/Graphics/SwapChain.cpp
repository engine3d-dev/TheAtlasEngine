#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/SwapChain.h>


namespace Engine3D{
    Ref<SwapChains> SwapChains::InitiateSwapChain(){
        switch (Pipeline::CurrentAPI()){
        case API::OPENGL:
        default:
            CoreLogError("API enabled is not supported, returning nullptr for InitiateSwapChain()");
            Render_Core_Assert(false);
            return nullptr;
        }

        CoreLogError("API enabled is not supported, returning nullptr for InitiateSwapChain()");
        Render_Core_Assert(false);
        return nullptr;
    }

    void SwapChains::SwapChainPresent(){
        CurrentSwapChainPresent();
    }
};