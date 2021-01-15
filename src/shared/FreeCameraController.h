#pragma once
#include "Event.h"
#include "Camera.h"

class FreeCameraController {
public:
    FreeCameraController() {}
    FreeCameraController(Camera& camera, float speed);

    void OnUpdate();
    void HandleEvent(Event& evt);
    void HandleKeyboardInput();

private:
    Camera* m_camera;
    float m_moveSpeed;

    bool m_moving = false;
};