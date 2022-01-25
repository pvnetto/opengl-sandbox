#include "05_Transform.h"

#include "shared/Texture2D.h"
#include "shared/Primitive.h"
#include <GLFW/glfw3.h>
#include <iostream>

void LOGL_05_Transform::OnAttach() {
	std::vector<Texture> textures {
	    TextureLoader::LoadTexture("../../src/assets/bricks.jpg", 0),
	    TextureLoader::LoadTexture("../../src/assets/yps.png", 1),
	};

	m_cube = Mesh{PrimitiveShape::Cube(), {}, textures};
    m_shader = Shader("../../src/shaders/04_vertex_transform.vert", "../../src/shaders/03_frag_tex.frag");

    // 1) Enables depth test, so vertices are stored on a z-buffer before drawing. This is required for 3D objects
	// glEnable(GL_DEPTH_TEST);
}

void LOGL_05_Transform::OnUpdate() {
	// 2) Enables depth buff clearing for 3D objects
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader.Use();
	m_shader.SetInt("tex", 0);
	m_shader.SetInt("anotherTex", 1);

	// Creates a transform matrix using GLM translate, rotate and scale functions.
	// BEWARE: Order is important!!!! You should always translate > rotate > scale.
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(glm::sin((float)glfwGetTime()) * 0.5f, 0.2f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(1.2f, 1.2f, 1.2f));
	// Passes transform matrix to the shader as a uniform
	m_shader.SetMatrix("transform", transform);

    m_cube.Draw(m_shader);
}
