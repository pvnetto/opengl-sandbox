#include "FreeCameraController.h"

#define ROTATION_SPEED 6.f

FreeCameraController::FreeCameraController(Camera &camera, float speed) : m_camera(&camera), m_moveSpeed(speed) {}

void FreeCameraController::HandleKeyboardInput(GLFWwindow *window, float deltaTime) {
	glm::vec3 moveDirection(0, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		moveDirection = m_camera->GetForward();
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		moveDirection = -m_camera->GetForward();
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		moveDirection = -m_camera->GetRight();
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		moveDirection = m_camera->GetRight();
	} else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		moveDirection = m_camera->GetUp();
	} else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		moveDirection = -m_camera->GetUp();
	}

	m_camera->AddPosition(moveDirection * m_moveSpeed * deltaTime);
}

void FreeCameraController::HandleMouseInput(GLFWwindow* window, glm::vec2 mouseDelta, float deltaTime) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
        glm::vec3 rotationDelta = glm::vec3(mouseDelta.y, mouseDelta.x, 0.f);
        m_camera->AddRotation(rotationDelta * ROTATION_SPEED * deltaTime);
    }
}