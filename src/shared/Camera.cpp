#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 rotationEuler, glm::vec3 up) {
	SetPosition(position);
	SetRotation(rotationEuler);

	m_worldUp = up;

	SetPerspective(75.0f, 800 / 600, 0.1f, 100.0f);
}

glm::mat4 Camera::GetView() const {
	// Calculates LookAt matrix manually instead of using glm. Check "Basics - Camera" docs for more info.
	glm::vec3 cameraDirectionInv = -m_forward;
	glm::vec3 cameraLeft = glm::normalize(glm::cross(m_worldUp, cameraDirectionInv));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirectionInv, cameraLeft));
	glm::mat4 basis(
	    glm::vec4(cameraLeft, 0),
	    glm::vec4(cameraUp, 0),
	    glm::vec4(cameraDirectionInv, 0),
	    glm::vec4(0, 0, 0, 1.f));
	basis = glm::transpose(basis);

	glm::mat4 translation(1.0f);
	translation = glm::translate(translation, -m_position);

	return basis * translation;
	// return glm::lookAt(m_position, m_position + m_forward, m_worldUp);
}

glm::vec3 Camera::GetRight() const {
	return glm::normalize(glm::cross(m_forward, m_worldUp));
}

glm::vec3 Camera::GetUp() const {
	return glm::normalize(glm::cross(GetForward(), GetRight()));
}

void Camera::SetRotation(glm::vec3 newRotation) {
	m_rotationEuler = newRotation;
	UpdateDirection();
}

void Camera::AddRotation(glm::vec3 rotationDelta) {
	m_rotationEuler += rotationDelta;
	UpdateDirection();
}

void Camera::UpdateDirection() {
	// Uses y > x > z gimbal order
	float yaw = glm::radians(m_rotationEuler.y);
	float pitch = glm::radians(m_rotationEuler.x);
	m_forward = glm::vec3(
	    std::cos(yaw) * std::cos(pitch),
	    std::sin(pitch),
	    std::sin(yaw) * std::cos(pitch));
	m_forward = glm::normalize(m_forward);
}

void Camera::SetOrthographic(float width, float height, float near, float far) {
	m_projection = glm::ortho(glm::radians(0.0f), width, 0.0f, height, near, far);
}

void Camera::SetPerspective(float fieldOfView, float aspectRatio, float near, float far) {
	m_projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, near, far);
}
