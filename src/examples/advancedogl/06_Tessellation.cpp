#include "06_Tessellation.h"

#include "shared/RenderUtils.h"
#include "shared/Utils.h"
#include "shared/renderer/OpenGL/Helpers.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include <iostream>


void AOGL_06_Tessellation::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 0.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	const glm::vec2 windowSize = spr::getWindowSize();
	const float aspectRatio = windowSize.x / windowSize.y;
	m_Camera.SetPerspective(90.f, aspectRatio, 0.1f, 100.f);

	m_QuadData = Utils::GetQuadData();

	glCreateBuffers(1, &m_QuadVertexBuffer);
	glNamedBufferStorage(m_QuadVertexBuffer, m_QuadData.VerticesSize * sizeof(float), m_QuadData.Vertices, 0);

	glCreateBuffers(1, &m_QuadIndexBuffer);
	glNamedBufferStorage(m_QuadIndexBuffer, m_QuadData.NumIndices * sizeof(unsigned int), m_QuadData.Indices, 0);

	glCreateVertexArrays(1, &m_VertexArray);
	glVertexArrayElementBuffer(m_VertexArray, m_QuadIndexBuffer);
	const int perVertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_VertexArray, perVertexBufferBindingPoint, m_QuadVertexBuffer, NULL, m_QuadData.Layout.getStride());
	for (int i = 0, count = m_QuadData.Layout.getAttributeCount(); i < count; i++) {
		const spr::VertexAttribute &layoutAttribute = m_QuadData.Layout.getAttribute(i);

		glVertexArrayAttribFormat(m_VertexArray, i, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
		glVertexArrayAttribBinding(m_VertexArray, i, perVertexBufferBindingPoint);

		glEnableVertexArrayAttrib(m_VertexArray, i);
	}

	// 0. Creates Program with Tessellation Shaders
	CreateShaderProgram();

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}

void AOGL_06_Tessellation::CreateShaderProgram() {
	m_TessellatedShaderProgram = glCreateProgram();

	CompileShaderAndAttach("shaders/default_unlit.vert",		GL_VERTEX_SHADER);
	CompileShaderAndAttach("shaders/wireframe.geom",			GL_GEOMETRY_SHADER);
	CompileShaderAndAttach("shaders/default_unlit.frag",		GL_FRAGMENT_SHADER);

	// 1. Compiles Control and Evaluation Shaders, and attaches them to the Shader Program
	if (m_bIsTessellationEnabled) {
		CompileShaderAndAttach("shaders/06_aogl_tess.control",		GL_TESS_CONTROL_SHADER);
		CompileShaderAndAttach("shaders/06_aogl_tess.eval",			GL_TESS_EVALUATION_SHADER);	
	}

	glLinkProgram(m_TessellatedShaderProgram);

	int programSuccess;
	glGetProgramiv(m_TessellatedShaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_TessellatedShaderProgram, 512, NULL, info);
		assert(false && "::ERROR: Failed to link Shader Program");
		return;
	}
}

void AOGL_06_Tessellation::CompileShaderAndAttach(const char *shaderPath, int shaderType) {
	const std::string shaderSrc = Utils::ReadShaderFile(shaderPath);
	const char *shaderSrcC = shaderSrc.c_str();
	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderSrcC, NULL);
	glCompileShader(shaderID);

	int bCompileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &bCompileStatus);
	if (!bCompileStatus) {
		char info[512];
		glGetShaderInfoLog(shaderID, 512, NULL, info);
		assert(false && "::ERROR: Failed to compile Fragment Shader");
		return;
	}

	glAttachShader(m_TessellatedShaderProgram, shaderID);
	glDeleteShader(shaderID);
}

void AOGL_06_Tessellation::OnUpdate() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model { 1.f };
	model = glm::translate(model, glm::vec3{ 0.f, 0.f, -5.f });
	model = glm::scale(model, glm::vec3{ 4.f, 4.f, 4.f });

	glUseProgram(m_TessellatedShaderProgram);
	unsigned int modelLocation = glGetUniformLocation(m_TessellatedShaderProgram, "model");
	unsigned int viewLocation = glGetUniformLocation(m_TessellatedShaderProgram, "view");
	unsigned int projectionLocation = glGetUniformLocation(m_TessellatedShaderProgram, "projection");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetProjection()));

	const glm::vec4 color { 1.f, 0.f, 1.f, 1.f };
	unsigned int colorLocation = glGetUniformLocation(m_TessellatedShaderProgram, "color");
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));

	// (OPTIONAL) Pass mouse position to shader as an uniform
	const glm::vec2 mousePos = spr::getMousePos();
	const glm::vec2 screenSize = spr::getWindowSize();
	const glm::vec2 screenPos = mousePos / screenSize;
	unsigned int mousePosLocation = glGetUniformLocation(m_TessellatedShaderProgram, "u_MousePos");
	glUniform2fv(mousePosLocation, 1, glm::value_ptr(screenPos));


	glBindVertexArray(m_VertexArray);

	// 3. Sets the number of vertices per-patch. We'll use triangles as patches, so we should have 3 vertices per-patch
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	// 4. Draws primitives as GL_PATCHES
	GLenum primitiveType = m_bIsTessellationEnabled ? GL_PATCHES : GL_TRIANGLES;
	glDrawElements(primitiveType, m_QuadData.NumIndices, GL_UNSIGNED_INT, 0);

	glCheckError();
}

void AOGL_06_Tessellation::OnDetach() {
	glDeleteProgram(m_TessellatedShaderProgram);
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_QuadVertexBuffer);
}
