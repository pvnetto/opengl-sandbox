#pragma once

#include "shared/FreeCameraController.h"
#include "shared/layers/Layer.h"
#include "shared/Light.h"

class LOGL_12_MultipleLights : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event &evt) override;

private:
	Camera m_Camera;
	FreeCameraController m_Controller;

    std::vector<DirectionalLight> m_DirectionalLights;
    PointLight m_PointLight;
    SpotLight m_Spotlight;
};