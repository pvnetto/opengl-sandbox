#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

class AOGL_02_Blending : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

private:
	Utils::ModelInstance m_QuadModel;

	spr::VertexBufferHandle m_CubeVertexBuffer;

	spr::ProgramHandle m_DefaultShaderProgram;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_ColorUniform;

private:
	Camera m_Camera;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;

private:
	enum OpaquePosition {
		OpaquePosition_Front = 0,
		OpaquePosition_Behind = 1,
	};

private:
	// Opaque objects should be rendered before the transparent ones, to avoid depth fragments.
	// e.g.,
	//  - Transparency Pass > Opaque Pass (❌): The Transparency Pass is executed first and writes to the Depth Buffer, so when the Opaque Pass
	// is executed, its fragments are Depth Tested and might be discarded instead of blended with the original colors.
	// 
	// - Opaque pass > Transparency pass (✔️): The Opaque Pass is executed first and writes to the Depth Buffer + Color Buffer, so when the Transparency
	// Pass takes place, even though it's Depth Tested, we don't have to worry because the Opaque colors are already at the Color Buffer and will be blended,
	// and if the Transparent objects are discarded that means they're behind the Opaque objects, which is fine.
	//
	// More about this:
	// - https://realtimecollisiondetection.net/blog/?p=86
	// - https://www.khronos.org/opengl/wiki/Transparency_Sorting#Depth_Sorting
	bool m_OpaqueFirst = true;
	int m_OpaquePosition = OpaquePosition_Behind;
};
