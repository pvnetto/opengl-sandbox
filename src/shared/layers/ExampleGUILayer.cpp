#include "ExampleGUILayer.h"

#include <imgui.h>

#include "examples/learnopengl/02_EBO.h"
#include "examples/learnopengl/02_VAO.h"
#include "examples/learnopengl/02_VBO.h"
#include "examples/learnopengl/03_Shaders.h"
#include "examples/learnopengl/04_Textures.h"
#include "examples/learnopengl/05_Transform.h"
#include "examples/learnopengl/06_MVP.h"
#include "examples/learnopengl/07_Camera.h"
#include "examples/learnopengl/08_Lighting.h"
#include "examples/learnopengl/09_Phong.h"
#include "examples/learnopengl/10_Material.h"
#include "examples/learnopengl/11_Specular_Map.h"
#include "examples/learnopengl/11_Emission_Map.h"
#include "examples/learnopengl/12_DirectionalLight.h"
#include "examples/learnopengl/12_PointLight.h"
#include "examples/learnopengl/12_SpotLight.h"
#include "examples/learnopengl/12_MultipleLights.h"
#include "examples/learnopengl/13_Meshes.h"
#include "examples/advancedogl/01_Framebuffers.h"
#include "examples/advancedogl/01_DepthBuffers.h"
#include "examples/advancedogl/01_StencilBuffers.h"
#include "examples/advancedogl/02_Blending.h"
#include "examples/advancedogl/03_Antialiasing.h"
#include "examples/advancedogl/04_Instancing.h"
#include "examples/advancedogl/05_GeometryShading.h"
#include "examples/advancedogl/06_Tessellation.h"
#include "examples/simplerenderer/01_Stenciling.h"
#include "examples/simplerenderer/02_OrderIndependentBlending.h"

#include "shared/Runtime.h"

#include <spr/SimpleRenderer.h>
#include <spw/SimpleWindow.h>

// clang-format off
const std::unordered_map<ExampleCategory, std::string> categoryStr{
    { LearnOpenGL,			"LearnOpenGL" },
	{ AdvancedOpenGL,		"Advanced OpenGL" },
	{ RendererExamples,		"Renderer Examples" },
};
// clang-format on

#define ADD_SIMPLE(category, type, name) m_examples[category].push_back(std::make_shared<SimpleExample<type>>(name))

ExampleGUILayer::ExampleGUILayer() : Layer("Example GUI") {
	m_examples.emplace(LearnOpenGL, std::vector<std::shared_ptr<Example>>());
	m_examples.emplace(AdvancedOpenGL, std::vector<std::shared_ptr<Example>>());

	ADD_SIMPLE(LearnOpenGL, LOGL_02_VBO,					"02 - VBO");
	ADD_SIMPLE(LearnOpenGL, LOGL_02_VAO,					"02 - VAO");
	ADD_SIMPLE(LearnOpenGL, LOGL_02_EBO,					"02 - EBO");
	ADD_SIMPLE(LearnOpenGL, LOGL_03_Shaders,				"03 - Shaders with uniforms");
	ADD_SIMPLE(LearnOpenGL, LOGL_04_Textures,				"04 - Textures");
	ADD_SIMPLE(LearnOpenGL, LOGL_05_Transform,				"05 - Transform");
	ADD_SIMPLE(LearnOpenGL, LOGL_06_MVP,					"06 - MVP");
	ADD_SIMPLE(LearnOpenGL, LOGL_07_Camera,					"07 - Camera");
	ADD_SIMPLE(LearnOpenGL, LOGL_08_Lighting,				"08 - Basic Lighting");
	ADD_SIMPLE(LearnOpenGL, LOGL_09_Phong,					"09 - Phong Shading");
	ADD_SIMPLE(LearnOpenGL, LOGL_10_Material,				"10 - Materials");
	ADD_SIMPLE(LearnOpenGL, LOGL_11_SpecularMap,			"11 - Specular Map");
	ADD_SIMPLE(LearnOpenGL, LOGL_11_EmissionMap,			"11 - Emission Map");
	ADD_SIMPLE(LearnOpenGL, LOGL_12_DirectionalLight,		"12 - Directional light");
	ADD_SIMPLE(LearnOpenGL, LOGL_12_PointLight,				"12 - Point light");
	ADD_SIMPLE(LearnOpenGL, LOGL_12_SpotLight,				"12 - Spotlight");
	ADD_SIMPLE(LearnOpenGL, LOGL_12_MultipleLights,			"12 - Multiple lights");
	ADD_SIMPLE(LearnOpenGL, LOGL_13_Meshes,					"13 - Meshes");

	ADD_SIMPLE(AdvancedOpenGL, AOGL_01_Framebuffers,		"01 - FrameBuffers");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_01_DepthBuffers,		"01 - FrameBuffers: Depth Testing");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_01_StencilBuffers,		"01 - FrameBuffers: Stencil Testing");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_02_Blending,			"02 - Blending");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_03_Antialiasing,		"03 - Antialiasing");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_04_Instancing,			"04 - Instancing");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_05_GeometryShading,		"05 - Geometry Shading");
	ADD_SIMPLE(AdvancedOpenGL, AOGL_06_Tessellation,		"06 - Tessellation Shading");

	ADD_SIMPLE(RendererExamples, SPRE_01_Stenciling,				"01 - Stenciling");
	ADD_SIMPLE(RendererExamples, SPRE_02_OrderIndependentBlending,	"02 - Order Independent Blending");
}

void ExampleGUILayer::OnImGuiRender() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float width = 300;

	const spw::Vec2 windowSize = spw::getWindowSize();
    ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x + windowSize.X - width, viewport->GetWorkPos().y));
    ImGui::SetNextWindowSize(ImVec2(width, viewport->GetWorkSize().y));

	// ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Examples");
	for (int i = 0; i < CategoryCount; i++) {
		if (ImGui::CollapsingHeader(categoryStr.at((ExampleCategory)i).c_str())) {
			for (auto example : m_examples.at((ExampleCategory)i)) {
				if (ImGui::Selectable(example->m_name.c_str(), m_selected == example)) {
					m_selected = example;
					Runtime::get()->replaceLayers(m_selected->GetLayers());
				}
			}
		}
	}
	ImGui::End();
}