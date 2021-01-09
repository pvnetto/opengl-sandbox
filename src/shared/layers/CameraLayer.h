#pragma once

#include "Layer.h"

#include "shared/Camera.h"
#include "shared/FreeCameraController.h"

#include <glm/glm.hpp>

class CameraLayer : public Layer {
public:
	CameraLayer(glm::vec3 position, glm::vec3 rotationEuler, glm::vec3 up);
	~CameraLayer();

	virtual void OnEvent(Event& evt) override;

	const Camera& GetCamera() { return camera; }

protected:
	Camera camera;
	FreeCameraController controller;
};