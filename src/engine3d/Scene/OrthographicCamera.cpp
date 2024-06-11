#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Scene/OrthographicCamera.h>

namespace engine3d{
    OrthographicCamera::OrthographicCamera(const CameraData& data) {
        // m_TransformationMatrix.view = glm::mat4(1.0);
        // m_TransformationMatrix.projection = glm::mat4(glm::ortho(m_Data.left, m_Data.right, m_Data.down, m_Data.up, -1.0f, 1.0f));
        // m_TransformationMatrix.model = m_TransformationMatrix.projection * m_TransformationMatrix.view;
    }

    void OrthographicCamera::SetProjection(float left, float right, float down, float up){
        // m_TransformationMatrix.projection = glm::ortho(left, right, down, up, -1.0f, 1.0f);
        UpdateViewMatrix();
    }

    void OrthographicCamera::UpdateViewMatrix(){
        // glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Data.position)
        //                       * glm::rotate(glm::mat4(1.0f), glm::radians(m_Data.rotation), glm::vec3(0, 0, 1));

        // m_TransformationMatrix.view = glm::inverse(transform);
        // m_TransformationMatrix.model = m_TransformationMatrix.projection * m_TransformationMatrix.view;
    }
};