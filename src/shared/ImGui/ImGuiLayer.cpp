#include "ImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include "shared/renderer/SimpleRenderer.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
	// --- examples/example_glfw_opengl3 ---
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	// --- examples/example_glfw_opengl3 ---


    // Initializes OpenGL and GLFW
	ImGui_ImplGlfw_InitForOpenGL(spr::getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayer::Begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End() {
	const auto windowSize = spr::getWindowSize();
	
	ImGuiIO &io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowSize.x, windowSize.y);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow *backupContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupContext);
	}
}

void ImGuiLayer::OnDettach() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
