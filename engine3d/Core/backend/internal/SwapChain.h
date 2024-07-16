#pragma once


namespace engine3d{
    /**
     * @name Swapchain
     * @note Graphics API-agnostic swap chain
     * 
    */
    class Swapchain{
    public:
        void InitializeSwapchain();

        void CleanupSwapchain();

    private:
    };
};