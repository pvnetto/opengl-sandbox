#include "04_Textures.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

void LOGL_04_Textures::OnAttach() {
	// 0) Declares VAO and VBOs + attributes
	float verticesA[] = {
	    // Triangle 1
	    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left
	    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // right
	    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top-right
	    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top-left
	};

	unsigned int indices[] = {
	    0, 1, 2,
	    2, 3, 0,
	};

	unsigned int vbo, ebo;
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbinds buffers so they're not accidentally used
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 3) Declares shader program
	m_shader = Shader("../../src/shaders/03_vertex_tex.vert", "../../src/shaders/03_frag_tex.frag");

	// 4) Loads textures
	int texWidth, texHeight, numOfChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *brickData = stbi_load("../../src/assets/bricks.jpg", &texWidth, &texHeight, &numOfChannels, 0);

	if (!brickData) {
		std::cout << "Couldn't load texture\n";
	}

	unsigned int brickTexture;
	glGenTextures(1, &brickTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, brickData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(brickTexture);

	stbi_image_free(brickData);

	// Loads another texture
	int tex2Width, tex2Height, tex2Channels;
	stbi_set_flip_vertically_on_load(true);     // BEWARE: Y coordinates are flipped on OpenGL, so textures must be flipped 
	unsigned char *tex2Data = stbi_load("../../src/assets/yps.png", &tex2Width, &tex2Height, &tex2Channels, 0);

	if (!tex2Data) {
		std::cout << "Couldn't load texture\n";
	}

	unsigned int tex2;
	glGenTextures(1, &tex2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2Width, tex2Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2Data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(tex2);

	stbi_image_free(tex2Data);
}

void LOGL_04_Textures::OnUpdate() {
	m_shader.Use();
	m_shader.SetInt("tex", 0);
	m_shader.SetInt("anotherTex", 1);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
