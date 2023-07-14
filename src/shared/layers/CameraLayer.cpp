#include "CameraLayer.h"

#include <functional>

CameraLayer::CameraLayer(glm::vec3 position, glm::vec3 rotationEuler, glm::vec3 up) : Layer("Camera Layer") {
	camera = Camera(position, rotationEuler, up);
	controller = FreeCameraController(camera, 4.5f);
}

CameraLayer::~CameraLayer() {}

void CameraLayer::OnEvent(Event& evt) {
	controller.HandleEvent(evt);
}