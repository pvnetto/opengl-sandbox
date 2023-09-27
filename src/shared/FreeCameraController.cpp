#include "FreeCameraController.h"

#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>
#include <spw/Inputs.h>
#include <spr/SimpleRenderer.h>

#define ROTATION_SPEED 6.f

FreeCameraController::FreeCameraController(Camera &camera, float speed) : m_Camera(&camera), m_moveSpeed(speed) {}

void FreeCameraController::HandleKeyboardInput() {
	glm::vec3 moveDirection { 0, 0, 0 };

	if (spw::isKeyPressed(spw::KEY_W) == spw::PRESS) {
		moveDirection = m_Camera->GetForward();
	}
	else if (spw::isKeyPressed(spw::KEY_S) == spw::PRESS) {
		moveDirection = -m_Camera->GetForward();
	}
	else if (spw::isKeyPressed(spw::KEY_A) == spw::PRESS) {
		moveDirection = -m_Camera->GetRight();
	}
	else if (spw::isKeyPressed(spw::KEY_D) == spw::PRESS) {
		moveDirection = m_Camera->GetRight();
	}
	else if (spw::isKeyPressed(spw::KEY_Q) == spw::PRESS) {
		moveDirection = m_Camera->GetUp();
	}
	else if (spw::isKeyPressed(spw::KEY_E) == spw::PRESS) {
		moveDirection = -m_Camera->GetUp();
	}

	m_Camera->AddPosition(moveDirection * m_moveSpeed * Runtime::get()->getDeltaTime());
}

void FreeCameraController::HandleEvent(const Event &evt) {
	if (const MouseMovedEvent *mouseEvt = dynamic_cast<const MouseMovedEvent *>(&evt)) {

		if (m_Moving) {
			// x, y are inverted for rotation
			const float currentDeltaTime = Runtime::get()->getDeltaTime();
			const glm::vec3 rotationDelta = glm::vec3(mouseEvt->MouseDelta().Y, mouseEvt->MouseDelta().X, 0.f);
			const glm::vec3 rotationVector = rotationDelta * ROTATION_SPEED * currentDeltaTime;
			m_Camera->AddRotation(rotationVector);
		}

		evt.m_Handled = true;
	}
	else if (const MouseButtonPressedEvent *pressedEvt = dynamic_cast<const MouseButtonPressedEvent *>(&evt)) {
		using namespace spw;
		
		if (pressedEvt->GetMouseButton() == MOUSE_BUTTON_RIGHT) {
			m_Moving = true;
			evt.m_Handled = true;
		}
	}
	else if (const MouseButtonReleasedEvent *releasedEvt = dynamic_cast<const MouseButtonReleasedEvent *>(&evt)) {
		using namespace spw;

		if (releasedEvt->GetMouseButton() == MOUSE_BUTTON_RIGHT) {
			m_Moving = false;
			evt.m_Handled = true;
		}
	}
}
