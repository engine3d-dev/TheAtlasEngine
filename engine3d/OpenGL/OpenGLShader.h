#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/Shader.h>
#include <filesystem>
#include <unordered_map>
#include <GLFW/glfw3.h>

namespace Engine3D{
    class OpenGLShader : public Shader{
    public:
        OpenGLShader(const std::filesystem::path& vertex, const std::filesystem::path& fragment);
        
        OpenGLShader(const std::filesystem::path& singleShaderSource);

        virtual ~OpenGLShader();

    private:
        //! @note  Should be able to load our single file shader
        //! @note Whether they are vertex shader/fragment shader
        std::string LoadFile(const std::string& filename);

        //! @note Loading a single file that contains both our vertex/fragment shaders
        //! @note This load function will make checks for that
        std::unordered_map<GLenum, std::string> LoadFromSingleFile(const std::string& srcfilename);

        //! @note Essentially allows doing ---- sources[type] =   CompileVertexShader(file);
        //! @note Where whether user passes in single .glsl file, raw string as vert and frag, or two seperate files
        //! @note Our shader class will handle this
        void CompileShaders(std::unordered_map<GLenum, std::string>& sources);

        // void CompileVertexShader(const std::string& vertex);
        // void CompileFragmentShader(const std::string& fragment);

    private:
        uint32_t Get(const std::string& name);

    private:
        std::string GetShaderNameID() const override;
        void ApplyCurrentBoundState() override;
        void ReleaseCurrentBoundState() override;
        void SetInt(const std::string& name, int value) override;
        void SetFloat(const std::string& name, float value) override;
        void SetFloat2(const std::string& name, const glm::vec2& values) override;
        void SetFloat3(const std::string& name, const glm::vec3& values) override;
        void SetFloat4(const std::string& name, const glm::vec4& values) override;
        void SetMat3(const std::string& name, const glm::mat3& values) override;
        void SetMat4(const std::string& name, const glm::mat4& values) override;

    private:
        uint32_t m_ProgramId;
        std::string m_Name;
    };
};