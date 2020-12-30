#pragma once
#include <GLFW/glfw3.h>
#include "Camera.h"

class FreeCameraController {
public:
    FreeCameraController(Camera& camera, float speed);

    void HandleKeyboardInput(GLFWwindow* window, float deltaTime);
    void HandleMouseInput(GLFWwindow* window, glm::vec2 mouseDelta, float deltaTime);

private:
    Camera* m_camera;
    float m_moveSpeed;
};