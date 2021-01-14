#include "03_Shaders.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void LOGL_03_Shaders::OnAttach() {
	// 0) Declares VAO and VBOs + attributes
	float verticesA[] = {
	    // Triangle 1
	    -0.8f, -0.2f, 0.0f, // left
	    -0.4f, -0.2f, 0.0f, // right
	    -0.6f, 0.2f, 0.0f,  // top
	};

	float verticesB[] = {
	    // Triangle 2
	    0.0f, -0.2f, 0.0f, // left
	    0.4f, -0.2f, 0.0f, // right
	    0.2f, 0.2f, 0.0f,  // top
	};

	unsigned int vboA, vboB;
	glGenVertexArrays(1, &m_vaoA);
	glGenBuffers(1, &vboA);

	// Binds VAO, so all subsequent buffers are bound to it
	glBindVertexArray(m_vaoA);

	// Binds VBO after VAO
	glBindBuffer(GL_ARRAY_BUFFER, vboA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

	// glVertexAttribPointer should tell the shader how to read buffer data
	// In this case we are declaring vec3 parameters as subsequent float values in an array,
	// so size = 3, type = float and stride = 3 * sizeof(float)
	// Also, Attributes are declared on slot 0, so the shader should use layout = 0 to find them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbinds buffers so they're not accidentally used
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Sets second VAO
	glGenVertexArrays(1, &m_vaoB);
	glGenBuffers(1, &vboB);
	glBindVertexArray(m_vaoB);

	glBindBuffer(GL_ARRAY_BUFFER, vboB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 3) Declares shader program
	m_shader = Shader("../../src/shaders/02_vertex_offset.vert", "../../src/shaders/frag_basic.frag");
}

void LOGL_03_Shaders::OnUpdate() {
	// 4) Use shader program, bind VAO and draw the triangle
	m_shader.Use();
	m_shader.SetFloat("horizontalOffset", std::sin(glfwGetTime()) * 1.0f);
	glBindVertexArray(m_vaoA);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(m_vaoB);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}