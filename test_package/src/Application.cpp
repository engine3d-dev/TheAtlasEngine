#include <engine3d/Core/Application.h>
#include <engine3d/Core/EntryPoint.h>
#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Scene/OrthographicCameraController.h>
#include <vector>

namespace Engine3D{
class ExampleLayer : public Layer{
public:
    ExampleLayer() : Layer("Sandbox Layer"), m_CameraController() {}

    //! @note  Attach() will be used to initiate our world
    void Attach() override {
        Pipeline::SetClearColor({0.05f, 0.05f, 0.05f, 1.0f});
        // m_CameraController = OrthographicCameraController();
        // float vertices[] = {
        //     0.5f,  0.5f, 0.0f,  // top right
        //     0.5f, -0.5f, 0.0f,  // bottom right
        // };

        // uint32_t indices[] = {
        //     0, 1, 3, // First Triangle
        //     1, 2, 3 // Second Triangle
        // };
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 
            0.5f, -0.5f, -0.5f,  
            0.5f,  0.5f, -0.5f,  
            0.5f,  0.5f, -0.5f,  
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 

            -0.5f, -0.5f,  0.5f, 
            0.5f, -0.5f,  0.5f,  
            0.5f,  0.5f,  0.5f,  
            0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f, -0.5f,  0.5f, 

            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f, 

            0.5f,  0.5f,  0.5f,  
            0.5f,  0.5f, -0.5f,  
            0.5f, -0.5f, -0.5f,  
            0.5f, -0.5f, -0.5f,  
            0.5f, -0.5f,  0.5f,  
            0.5f,  0.5f,  0.5f,  

            -0.5f, -0.5f, -0.5f, 
            0.5f, -0.5f, -0.5f,  
            0.5f, -0.5f,  0.5f,  
            0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f, 
            -0.5f, -0.5f, -0.5f, 

            -0.5f,  0.5f, -0.5f, 
            0.5f,  0.5f, -0.5f,  
            0.5f,  0.5f,  0.5f,  
            0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f
        };

        uint32_t indices[] = {
            0, 1, 3, // First Triangle
            1, 2, 3 // Second Triangle
        };

        cubePositions = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };

        m_Vbo = VertexBuffer::Create(vertices, sizeof(vertices));
        
        //! @note Shader Data Layout used by vertex array
        //! @note For setting up how our primitives gets rendered by specifying the types.
        //! @note To get a color on the primitive, pass in the FragColor type
        // m_Vbo->SetLayout({
        //     { ShaderData_t::Float3, "aPos" }, // layout(location = 0)
        //     { ShaderData_t::Float4, "aColor" },
        //     { ShaderData_t::Float2, "aTexCoord" }
        // });
        m_Vbo->SetLayout({
            { ShaderData_t::Float3, "aPos" }, // layout(location = 0)
        });

        // m_Ibo = IndexBuffer::Create(indices, sizeof(indices));

        m_Vao = VertexArray::Create();
        m_Vao->AddVertexBuffer(m_Vbo);
        // m_Vao->SetIndexBuffer(m_Ibo);

        m_CubeVao = VertexArray::Create();
        m_CubeVbo = VertexBuffer::Create(vertices, sizeof(vertices));
        m_CubeVbo->SetLayout({
            { ShaderData_t::Float3, "aPos" }, // layout(location = 0)
        });

        m_CubeVao->AddVertexBuffer(m_CubeVbo);
        // m_CubeVao->SetIndexBuffer(m_Ibo);
        m_CubeVao->ApplyResource();

        Ref<Shader> lightShader = Shader::Create("Resources/shaders/LightTutorialShader/Light.glsl");
        Ref<Shader> cubeShader = Shader::Create("Resources/shaders/LightTutorialShader/Cube.glsl");
        // m_ShaderLib.Add(shader);
        m_ShaderLib.Add("Cube", cubeShader);
        m_ShaderLib.Add("Light", lightShader);

        m_Texture = Texture2D::Create("Resources/textures/brick.jpg");

        if(!m_Texture->IsLoaded()){
            CoreLogWarn("Could not load texture!");
        }

    }

    void Detach() override {}

    void UpdateEvent(Event& event) override {
        m_CameraController.OnEvent(event);
    }

    void UpdateLayer(Timestep ts) override {
        m_CameraController.OnUpdate(ts);

        auto lightShader = m_ShaderLib.Get("Light");
        auto cubeShader = m_ShaderLib.Get("Cube");


        lightShader->Set("objectColor", {1.0f, 1.0f, 0.5f});
        lightShader->Set("lightColor", {1.0f, 1.0f, 1.0f});

        cubeShader->Set("projection", m_CameraController.GetProjectionMatrix());
        cubeShader->Set("view", m_CameraController.GetViewMatrix());
        cubeShader->Set("objectColor", m_ObjectColor);
        cubeShader->Set("lightColor", m_LightColor);

        glm::mat4 model = glm::mat4(1.0f);
        cubeShader->Set("model", model);

        Renderer::RenderPass(m_CubeVao, 36);


        lightShader->Set("projection", m_CameraController.GetProjectionMatrix());
        lightShader->Set("view", m_CameraController.GetViewMatrix());

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader->Set("model", model);

        Renderer::RenderPass(m_CubeVao, 36);
    }

    void UIRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Object", glm::value_ptr(m_ObjectColor));
        ImGui::ColorEdit3("Light", glm::value_ptr(m_LightColor));
        ImGui::End();
    }

private:
    Ref<VertexBuffer> m_Vbo;
    Ref<VertexBuffer> m_CubeVbo;

    // Ref<IndexBuffer> m_Ibo;
    Ref<VertexArray> m_Vao;

    Ref<VertexArray> m_CubeVao;

    ShaderLibrary m_ShaderLib;
    Ref<Texture2D> m_Texture;
    OrthographicCameraController m_CameraController;
    std::vector<glm::vec3> cubePositions;

    glm::vec4 testColor = {0.0f, 0.0f, 1.0f, 1.0f};
    glm::vec3 lightPos = {1.2f, 1.0f, 2.0f};

    glm::vec3 m_ObjectColor = {0.0f, 0.0f, 1.0f};
    glm::vec3 m_LightColor = {1.0f, 1.0f, 1.0f};

};

class UIApplication : public Application{
public:
    UIApplication() : Application("UIApplication") {
        PushLayer<ExampleLayer>();
    }

private:
};

Application* CreateApplication(){
    return new UIApplication();
}

};