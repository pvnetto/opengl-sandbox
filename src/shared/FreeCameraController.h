#pragma once

#include "Camera.h"

#include <spw/Event.h>

class FreeCameraController {
public:
    FreeCameraController() {}
    FreeCameraController(Camera& camera, float speed);

    void OnUpdate();
    void HandleEvent(const Event& evt);
    void HandleKeyboardInput();

private:
    Camera* m_Camera;
    float m_moveSpeed;

    bool m_moving = false;
};