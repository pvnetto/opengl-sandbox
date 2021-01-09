#pragma once
#include "Event.h"
#include "Camera.h"

class FreeCameraController {
public:
    FreeCameraController() {}
    FreeCameraController(Camera& camera, float speed);

    void HandleKeyboardInput(float deltaTime);

    void OnUpdate();
    void HandleEvent(Event& evt);

private:
    Camera* m_camera;
    float m_moveSpeed;

    bool m_moving = false;
};