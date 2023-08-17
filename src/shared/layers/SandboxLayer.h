#pragma once

#include "Layer.h"
#include "shared/RenderUtils.h"

class SandboxLayer : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

private:
	Utils::ModelInstance m_Model;

	spr::ProgramHandle m_ShaderProgram;
	spr::UniformHandle m_TimeUniform;
	spr::UniformHandle m_ResolutionUniform;

};