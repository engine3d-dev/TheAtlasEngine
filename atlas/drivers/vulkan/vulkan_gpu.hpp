#pragma once
#include <functional>

namespace atlas::vk{
    /**
     * @note These operations provide a way to directly execute commands to the GPU.
     * @note These operations should not be called directly outside of the the vulkan's implementation
     * @note As these implementations are specific to the vulkan API
     * @note These submission functions enable us to not only submit our data to the GPU directly, but ensures that the tasks we submit are going to be in sync
     * 
    vk::Submit([]{
        // This directly sends our information task directly to the GPU
    });


    @note SubmitResourceFree allows us to submit tasks that are used for cleanup
    @note While also ensuring that the operations in which these are submitted would be handled for us internally
    @note the deletion queue will probably be implemented with the vulkan driver internally
    vk::SubmitResourceFree([]{
        // This directly gets submitted to a queue for handling cleanup for the specific operation
        // Use case is if we wanted to cleanup imgui when at deconstruction for when the application ends, they get called here, and then added to queue for cleanup
    });
    */
    
    void Submit(const std::function<void()>& Function);

    void SubmitResourceFree(const std::function<void()>& Function);

    /**
    
    @note Descriptor Sets Layouts

    vk::DescriptorSetConfigurations config = {
    };
    
    */
};