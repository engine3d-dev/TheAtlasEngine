#pragma once
#include <string>
#include <vector>

namespace engine3d{
    /**
     * @name VulkanShader-Experiemental
     * @note "-Experiemental" means that this file is still experiementing new API design, logic, and various other extensive testing
    */
    namespace vk{
        class VulkanShader{
        public:
            //! @note Loading a vertex and fragment shader
            VulkanShader(const std::string& vertexShader, const std::string& fragmentShader);

            //! @note Validating if shaders are loaded successfully
            bool IsLoaded();

            //! @note This is where the set functions will be located
            // void SetInt(const std::string& name, int value);


        private:    
            //! @note Parsing and loading .sprirv files.
            std::vector<char> LoadingShader(const std::string& filename);

        private:
            bool m_IsVertexShaderLoadedSuccessfull = false;
            bool m_IsFragmentShaderLoadedSuccessfull = false;
        };
    };
};