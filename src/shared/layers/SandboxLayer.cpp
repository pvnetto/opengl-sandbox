#include "SandboxLayer.h"

#include "shared/Window.h"

void SandboxLayer::OnAttach() {
    m_shader = Shader("../../src/shaders/sandbox_vertex.vert", "../../src/shaders/sandbox_fragment.frag");
    m_quad = Model("../../src/assets/quad/quad.obj");
}

void SandboxLayer::OnUpdate() {
    m_shader.SetFloat("iTime", (float) glfwGetTime());
    m_shader.SetVector2("iResolution", Window::Get()->GetSize());
    m_quad.Draw(m_shader);
}

void SandboxLayer::OnImGuiRender() {

}