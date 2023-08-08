#include "SandboxLayer.h"

#include "shared/renderer/SimpleRenderer.h"
#include <GLFW/glfw3.h>

void SandboxLayer::OnAttach() {
    m_shader = Shader("shaders/sandbox_vertex.vert", "shaders/sandbox_fragment.frag");
    m_quad = Model("assets/quad.obj");
}

void SandboxLayer::OnUpdate() {
    m_shader.SetFloat("iTime", (float) glfwGetTime());
    m_shader.SetVector2("iResolution", spr::getWindowSize());
    m_quad.Draw(m_shader);
}

void SandboxLayer::OnImGuiRender() {

}
