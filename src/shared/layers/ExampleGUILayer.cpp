#include "ExampleGUILayer.h"

#include <imgui.h>

#include "shared/Window.h"
#include "examples/learnopengl/02_EBO.h"
#include "examples/learnopengl/13_Meshes.h"

const std::unordered_map<ExampleCategory, std::string> categoryStr{
    {LearnOpenGL, "LearnOpenGL"}, {BookOfShaders, "Book Of Shaders"}, {Trigonometry, "Trigonometry"}, {Matrices, "Matrices"}};

ExampleGUILayer::ExampleGUILayer() : Layer("Example GUI") {
	m_examples.emplace(LearnOpenGL, std::vector<std::shared_ptr<Example>>());
    m_examples.emplace(BookOfShaders, std::vector<std::shared_ptr<Example>>());
    m_examples.emplace(Trigonometry, std::vector<std::shared_ptr<Example>>());
    m_examples.emplace(Matrices, std::vector<std::shared_ptr<Example>>());

	m_examples[LearnOpenGL].push_back(std::make_shared<SimpleExample<LOGL_02_EBO>>("02 - EBO"));
    m_examples[LearnOpenGL].push_back(std::make_shared<SimpleExample<LOGL_13_Meshes>>("13 - Meshes"));
}

void ExampleGUILayer::OnImGuiRender() {
	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	for (int i = 0; i < CategoryCount; i++) {
		if (ImGui::CollapsingHeader(categoryStr.at((ExampleCategory)i).c_str())) {
			for (auto example : m_examples.at((ExampleCategory)i)) {
                if(ImGui::Selectable(example->m_name.c_str(), m_selected == example)) {
                    m_selected = example;
                    Window::Get()->ScheduleReset(m_selected->GetLayers());
                }
            }
		}
	}
}