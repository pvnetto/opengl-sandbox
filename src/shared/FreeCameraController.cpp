#include "FreeCameraController.h"
#include "shared/renderer/SimpleRenderer.h"
#define ROTATION_SPEED 6.f

FreeCameraController::FreeCameraController(Camera &camera, float speed) : m_camera(&camera), m_moveSpeed(speed) {}

void FreeCameraController::HandleKeyboardInput() {
	glm::vec3 moveDirection(0, 0, 0);

	auto window = spr::getWindow();
	if (spr::input::pressedKey(SPR_KEY_W) == SPR_PRESS) {
		moveDirection = m_camera->GetForward();
	}
	else if (spr::input::pressedKey(SPR_KEY_S) == SPR_PRESS) {
		moveDirection = -m_camera->GetForward();
	}
	else if (spr::input::pressedKey(SPR_KEY_A) == SPR_PRESS) {
		moveDirection = -m_camera->GetRight();
	}
	else if (spr::input::pressedKey(SPR_KEY_D) == SPR_PRESS) {
		moveDirection = m_camera->GetRight();
	}
	else if (spr::input::pressedKey(SPR_KEY_Q) == SPR_PRESS) {
		moveDirection = m_camera->GetUp();
	}
	else if (spr::input::pressedKey(SPR_KEY_E) == SPR_PRESS) {
		moveDirection = -m_camera->GetUp();
	}

	m_camera->AddPosition(moveDirection * m_moveSpeed * spr::runtime::getDeltaTime());
}

void FreeCameraController::HandleEvent(Event &evt) {
	if (MouseMovedEvent *mouseEvt = dynamic_cast<MouseMovedEvent *>(&evt)) {

		if (m_moving) {
			// x, y are inverted for rotation
			glm::vec3 rotationDelta = glm::vec3(mouseEvt->MouseDelta().y, mouseEvt->MouseDelta().x, 0.f);
			m_camera->AddRotation(rotationDelta * ROTATION_SPEED * spr::runtime::getDeltaTime());
		}

		evt.m_handled = true;
	}
	else if (MouseButtonPressedEvent *pressedEvt = dynamic_cast<MouseButtonPressedEvent *>(&evt)) {
		if (pressedEvt->GetMouseButton() == GLFW_MOUSE_BUTTON_2) {
			m_moving = true;
			evt.m_handled = true;
		}
	}
	else if (MouseButtonReleasedEvent *releasedEvt = dynamic_cast<MouseButtonReleasedEvent *>(&evt)) {
		if (releasedEvt->GetMouseButton() == GLFW_MOUSE_BUTTON_2) {
			m_moving = false;
			evt.m_handled = true;
		}
	}
}