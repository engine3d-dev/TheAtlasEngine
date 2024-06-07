#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/Shader.h>
#include <engine3d/OpenGL/OpenGLShader.h>

namespace Engine3D{
    Ref<Shader> Shader::Create(const std::filesystem::path& vertex, const std::filesystem::path& fragment){
        switch(Pipeline::CurrentAPI()){
            case API::OPENGL: return CreateRef<OpenGLShader>(vertex, fragment);
            case API::VULKAN: CoreLogError("Vulkan not implemented Shader"); break;
            case API::DIRECTX11: CoreLogError("DirectX12 not implemented Shader"); break;
            case API::DIRECT12: CoreLogError("DirectX12 not implemented Shader"); break;
        }

        CoreLogError("API that was set is unsupported");
        assert(false);
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::filesystem::path& singleShaderSource){
        switch(Pipeline::CurrentAPI()){
            case API::OPENGL: return CreateRef<OpenGLShader>(singleShaderSource);
            case API::VULKAN: CoreLogError("Vulkan not implemented Shader"); break;
            case API::DIRECTX11: CoreLogError("DirectX12 not implemented Shader"); break;
            case API::DIRECT12: CoreLogError("DirectX12 not implemented Shader"); break;
        }

        CoreLogError("API that was set is unsupported");
        assert(false);
        return nullptr;
    }

    std::string Shader::GetShaderName() const { return GetShaderNameID(); }

    void Shader::ApplyResources(){
        ApplyCurrentBoundState();
    }

    void Shader::ReleaseResources(){
        ReleaseCurrentBoundState();
    }

    void Shader::Set(const std::string& name, int value) {
        SetInt(name, value);
    }

    void Shader::Set(const std::string& name, float value) {
        SetFloat(name, value);
    }

    void Shader::Set(const std::string& name, const glm::vec2& values) {
        SetFloat2(name, values);
    }

    void Shader::Set(const std::string& name, const glm::vec3& values) {
        SetFloat3(name, values);
    }

    void Shader::Set(const std::string& name, const glm::vec4& values){
        SetFloat4(name, values);
    }

    void Shader::Set(const std::string& name, const glm::mat3& values) {
        SetMat3(name, values);
    }

    void Shader::Set(const std::string& name, const glm::mat4& values) {
        SetMat4(name, values);
    }

    // **********************************************
    //               [ Shader Library ]
    // **********************************************

    void ShaderLibrary::Add(const Ref<Shader>& shader){
        std::string key = shader->GetShaderName();
        m_Shaders[key] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filename){
        Ref<Shader> shader = Shader::Create(filename);
        std::string key = shader->GetShaderName();
        Add(shader);
        return shader;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader){
        if(IsShaderExists(name)){
            CoreLogWarn("Shader with name {} already exists", name);
            return;
        }
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name){
        if(!IsShaderExists(name)){
            CoreLogWarn("Shader named {} does not exist. Cannot be extracted by ShaderLibrary::Get", name);
            assert(false);
        }

        return m_Shaders[name];
    }

    bool ShaderLibrary::IsShaderExists(const std::string& name){
        return m_Shaders.contains(name);
    }
};