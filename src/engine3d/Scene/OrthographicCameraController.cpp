#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Scene/OrthographicCameraController.h>

namespace Engine3D{
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    OrthographicCameraController::OrthographicCameraController(){
        m_ViewMatrix = glm::mat4(1.0f);
        // m_ProjectionMatrix = glm::mat4(1.0f);
        m_AspectRatio = Application::Get().WindowSize().x / Application::Get().WindowSize().x;
        m_CameraSpeed = 0.05f;
        m_Yaw = -90.0f;
        m_Pitch = 0.0f;
        m_LastCoords = {Application::WindowSize().x / 2.0f, Application::WindowSize().y / 2.0f};
        m_Fov = 45.0f;
        m_FirstMouse = true;
        m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    void OrthographicCameraController::OnEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event){
            OnMouseScroll(event.GetXOffset(), event.GetYOffset());
            return false;
        });
    }

    // glm::mat4 OrthographicCameraController::GetModelMatrix(const glm::vec3& position){

    // }

    void OrthographicCameraController::OnMouseScroll(double xOffset, double yOffset){
        m_Fov = (float)yOffset;
        if(m_Fov < 1.0f) m_Fov = 1.0f;
        if(m_Fov > 45.0f) m_Fov = 45.0f;
    }

    void OrthographicCameraController::OnUpdate(Timestep ts){
        if(InputPoll::IsKeyPressed(ENGINE_KEY_W)){
            m_CameraPosition += m_CameraSpeed * m_CameraFront;
        }
        else if(InputPoll::IsKeyPressed(ENGINE_KEY_S)){
            m_CameraPosition -= m_CameraSpeed * m_CameraFront;
        }
        else if(InputPoll::IsKeyPressed(ENGINE_KEY_A)){
            m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
        }
        else if(InputPoll::IsKeyPressed(ENGINE_KEY_D)){
            m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
        }

        
        if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_RIGHT)){
            OnMouseUpdate();
        }
    }


    void OrthographicCameraController::OnMouseUpdate(){
        //! @note TODO ---- When user clicks Q - goes upwards and E - goes downwards
        //! @note TODO ---- Viewport control, when mouse goes out of bounds do not run this OnMouseUpdate
        double xPosInput = InputPoll::GetMousePos().x, yPosInput = InputPoll::GetMousePos().y;
        float xpos = static_cast<float>(xPosInput);
        float ypos = static_cast<float>(yPosInput);

        if(m_FirstMouse){
            m_LastCoords.x = xpos;
            m_LastCoords.y = ypos;
            m_FirstMouse = false;
        }

        float xOffset = xpos - m_LastCoords.x;
        float yOffset = m_LastCoords.y - ypos;

        m_LastCoords.x = xpos;
        m_LastCoords.y = ypos;

        const float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        m_Yaw += xOffset;
        m_Pitch += yOffset;

        if(m_Pitch > 89.0f) m_Pitch = 89.0f;
        if(m_Pitch < -89.0f) m_Pitch = -89.0f;

        glm::vec3 direction;
        direction.x = std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));
        direction.y = std::sin(glm::radians(m_Pitch));
        direction.z = std::sin(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));
        m_CameraFront = glm::normalize(direction);
    }

    glm::mat4 OrthographicCameraController::GetViewMatrix() const {
        view = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);
        return view;
    }

    glm::mat4 OrthographicCameraController::GetProjectionMatrix() const {
        projection = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);
        return projection;
    }
    // static float g_AspectRatio = (Application::Get().WindowSize().x/Application::Get().WindowSize().y);

    // OrthographicCameraController::OrthographicCameraController(bool rotation) : m_AspectRatio(Application::Get().WindowSize().x/Application::Get().WindowSize().y), m_IsRotationEnabled(rotation), m_ZoomLevel(1.0f), m_OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {
    // }

    // void OrthographicCameraController::OnEvent(Event& event) {
    //     EventDispatcher dispatcher(event);

    //     dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event){
    //         m_ZoomLevel -= event.GetYOffset() * 0.25f;
    //         m_ZoomLevel = std::max(m_ZoomLevel, 1.0f);

    //         OnUpdateView();
    //         return false;
    //     });

    //     dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event){
    //         OnResize((float)event.GetWidth(), (float)event.GetHeight());
    //         return false;
    //     });
    // }

    // void OrthographicCameraController::OnUpdate(Timestep ts) {
    //     if(InputPoll::IsKeyPressed(ENGINE_KEY_D)){ // RIGHT
    //         CoreLogTrace("D Pressed!");
	// 		m_CameraPosition.x -= std::cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	// 		m_CameraPosition.y -= std::sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    //     }
    //     else if(InputPoll::IsKeyPressed(ENGINE_KEY_A)){ // LEFT
    //     CoreLogTrace("A Pressed!");
	// 		m_CameraPosition.x += std::cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	// 		m_CameraPosition.y += std::sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    //     }
    //     else if(InputPoll::IsKeyPressed(ENGINE_KEY_W)){ // UP
    //         CoreLogTrace("S Pressed!");
	// 		m_CameraPosition.x += -std::sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	// 		m_CameraPosition.y += std::cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    //     }
    //     else if(InputPoll::IsKeyPressed(ENGINE_KEY_A)){ // DOWN
    //         CoreLogTrace("D Pressed!");
	// 		m_CameraPosition.x -= -std::sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	// 		m_CameraPosition.y -= std::cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    //     }
		
	// 	// Checking if we set the rotation speed
	// 	if(m_IsRotationEnabled){

	// 		if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)){ // Rotate Right
	// 			m_CameraRotation -= m_CameraRotationSpeed * ts;
	// 		}

	// 		if(InputPoll::IsKeyPressed(ENGINE_KEY_E)){ // Rotate Left
	// 			m_CameraRotation += m_CameraRotationSpeed * ts;
	// 		}
			
	// 		if(m_CameraRotation > 180.0f)
	// 			m_CameraRotation -= 360.0f;
	// 		else if(m_CameraRotation <= -180.0f)
	// 			m_CameraRotation += 360.0f;

	// 		// Then we set the camera rotation
	// 		m_OrthographicCamera.SetRotation(m_CameraRotation);
	// 	}
		
	// 	m_OrthographicCamera.SetPosition(m_CameraPosition);
	// 	m_CameraTranslationSpeed = m_ZoomLevel;
    // }
    
    // void OrthographicCameraController::OnResize(float w, float h) {
    //     m_AspectRatio = w / h;
    //     OnUpdateView();
    // }

    // OrthographicCamera& OrthographicCameraController::GetCamera() { return m_OrthographicCamera;}
    // OrthographicCamera OrthographicCameraController::GetCamera() const { return m_OrthographicCamera; }

    // glm::mat4 OrthographicCameraController::GetViewMatrix() const { return m_OrthographicCamera.GetViewMatrix(); }
    // glm::mat4 OrthographicCameraController::GetProjectionMatrix() const { return m_OrthographicCamera.GetProjectionMatrix(); }

    // void OrthographicCameraController::SetZoomLevel(float level) {
    //     m_ZoomLevel = level;
    //     OnUpdateView();
    // }

    // float OrthographicCameraController::GetZoomLevel() const { return m_ZoomLevel; }

    // void OrthographicCameraController::OnUpdateView(){
    //     float left = -m_AspectRatio * m_ZoomLevel, right = m_AspectRatio * m_ZoomLevel, bottom = -m_ZoomLevel, top = m_ZoomLevel;
    //     m_OrthographicCamera.SetProjection(left, right, bottom, top);
    // }
};