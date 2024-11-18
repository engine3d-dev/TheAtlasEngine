#pragma once
#include <Core/Core.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    /**
     * @name GraphicSwapchain
     * @note Defines what our swapchain is going to be.
     * @note Reason for this is because swapchain's API can vary if using vulkan's swapchain or DirectX3D 11/12
    */
    // class GraphicSwapchain;
    class graphic_swapchain;

    /**
     * @name GraphicPhysicalDeviceDriver
     * @note Defines the physical GPU hardware.
     * @note Fetches the GPU information.
     * @note Notifies our GraphicDriver what GPU's drivers are available from the vendors.
     * @note Enable selection of GPU's to submit data to.
     * @note Probably use some set<T> or map<T> to check if specific device extensions are available on these GPU's
     * @note The actual interaction to how the literal physical GPU is going to deal with our data at the last stage.
    */
    // class GraphicPhysicalDriver;
    class graphic_physical_driver;

    /**
     * @name GraphicDriver
     * @note Defines at the software level how data may the GPU use.
     * @note Also defines what driver we want to submit our data to.
     * @note In the moments that there may be multiple graphic_driver, we may utilize this or multiple instances of this.
    */
    // class GraphicDriver;
    class graphic_driver;

    /**
     * @name Graphic Context Driver
     * @note This defines our abstraction of other backend.
     * @note Backend refers to our graphics API. Such as Vulkan, Metal, or DirectX11/12
    */
    // class GraphicContextDriver;
    class graphic_context_driver{
    public:
        //! @note Used to pass our instance
        static VkInstance& GetInstance();

        //! @note Passing pointers to our device/swapchain
        static graphic_driver* CurrentDevice();
        static graphic_swapchain* CurrentSwapchain();

        
    };
};