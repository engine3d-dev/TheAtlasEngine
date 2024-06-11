#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Renderer/Renderer.h>

namespace engine3d{
    // void Renderer::RenderPass(Ref<VertexArray>& vao){
    //     vao->ApplyResource();
    //     glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    // }

    void Renderer::RenderPass(Ref<VertexArray>& vao, size_t count){
        vao->ApplyResource();
        if(vao->GetIndexBuffer()){
            glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }
        else{
            glDrawArrays(GL_TRIANGLES, 0, count);
        }
    }
};