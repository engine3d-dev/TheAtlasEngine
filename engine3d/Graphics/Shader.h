#pragma once
#include <engine3d/Core/core.h>
#include <filesystem>
#include "glm/glm.hpp"

namespace engine3d{
    /**
     * @name Shader
     * @note Our client API will call these functions
     * @note Our implementation details will be privated and implemented by our graphics API
     * @note This is API-agnostic shader interface
    */
    class Shader{
    public:
        virtual ~Shader() = default;

        static Ref<Shader> Create(const std::filesystem::path& vertex, const std::filesystem::path& fragment);

        //! @note  This will accept if user puts entire shader that contains vertex/fragment shader
        static Ref<Shader> Create(const std::filesystem::path& singleShaderSource);

        void Set(const std::string& name, int value);
        void Set(const std::string& name, float value);
        void Set(const std::string& name, const glm::vec2& values);
        void Set(const std::string& name, const glm::vec3& values);
        void Set(const std::string& name, const glm::vec4& values);
        void Set(const std::string& name, const glm::mat3& values);
        void Set(const std::string& name, const glm::mat4& values);

        void ApplyResources();
        void ReleaseResources();

        std::string GetShaderName() const;

    private:
        virtual std::string GetShaderNameID() const = 0;
        virtual void ApplyCurrentBoundState() = 0;
        virtual void ReleaseCurrentBoundState() = 0;
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& values) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& values) = 0;
    };

    class ShaderLibrary{
    public:
        //! @note Loads brand new shader and then add to our map
        Ref<Shader> Load(const std::string& filename);

        //! @note Adding our shader to our library
        void Add(const Ref<Shader>& shader);

        //! @note Adding our shader and assigning a name to that shader
        void Add(const std::string& name, const Ref<Shader>& shader);

        Ref<Shader> Get(const std::string& name);

    private:
        bool IsShaderExists(const std::string& name);
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
};