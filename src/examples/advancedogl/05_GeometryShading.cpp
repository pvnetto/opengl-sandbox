#include "05_GeometryShading.h"

#include "shared/RenderUtils.h"
#include "shared/Utils.h"
#include "spr/OpenGL/Helpers.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include <iostream>

void AOGL_05_GeometryShading::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 0.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	const spw::Vec2 windowSize = spw::getWindowSize();
	const float aspectRatio = (float) windowSize.X / windowSize.Y;
	m_Camera.SetPerspective(90.f, aspectRatio, 0.1f, 100.f);

	m_CubeData = Utils::GetCubeData();

	glCreateBuffers(1, &m_MeshVertexBuffer);
	glNamedBufferStorage(m_MeshVertexBuffer, m_CubeData.VerticesSize * sizeof(float), m_CubeData.Vertices, 0);

	glCreateVertexArrays(1, &m_VertexArray);
	const int perVertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_VertexArray, perVertexBufferBindingPoint, m_MeshVertexBuffer, NULL, m_CubeData.Layout.getStride());
	for (int i = 0, count = m_CubeData.Layout.getAttributeCount(); i < count; i++) {
		spr::VertexAttribute layoutAttribute;
		if (m_CubeData.Layout.getAttribute(i, layoutAttribute)) {
			glVertexArrayAttribFormat(m_VertexArray, i, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
			glVertexArrayAttribBinding(m_VertexArray, i, perVertexBufferBindingPoint);

			glEnableVertexArrayAttrib(m_VertexArray, i);		
		}
	}

	// 0. Creates Program with a Geometry Shader
	CreateShaderProgram();

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}

void AOGL_05_GeometryShading::CreateShaderProgram() {
	unsigned int vertexShader, geometryShader, fragmentShader;

	{
		const std::string vertexSrc = Utils::ReadShaderFile("shaders/default_unlit.vert");
		const char *vertexSrcC = vertexSrc.c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSrcC, NULL);
		glCompileShader(vertexShader);

		int vertSuccess;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
		if (!vertSuccess) {
			char info[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, info);
			assert(false && "::ERROR: Failed to compile Vertex Shader");
			return;
		}
	}

	// 1. Compiles Geometry Shader
	{
		const std::string geometrySrc = Utils::ReadShaderFile("shaders/wireframe.geom");
		const char *geometrySrcC = geometrySrc.c_str();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometrySrcC, NULL);
		glCompileShader(geometryShader);

		int geomSuccess;
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geomSuccess);
		if (!geomSuccess) {
			char info[512];
			glGetShaderInfoLog(geometryShader, 512, NULL, info);
			assert(false && "::ERROR: Failed to compile Geometry Shader");
			return;
		}
	}

	{
		const std::string fragSrc = Utils::ReadShaderFile("shaders/default_unlit.frag");
		const char *fragSrcC = fragSrc.c_str();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragSrcC, NULL);
		glCompileShader(fragmentShader);

		int fragSuccess;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
		if (!fragSuccess) {
			char info[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, info);
			assert(false && "::ERROR: Failed to compile Fragment Shader");
			return;
		}
	}

	// 2. Attaches Geometry Shader to the Program
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, geometryShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);

	int programSuccess;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, info);
		assert(false && "::ERROR: Failed to link Shader Program");
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void AOGL_05_GeometryShading::OnUpdate() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model{1.f};
	model = glm::translate(model, glm::vec3{0.f, 0.f, -5.f});
	model = glm::scale(model, glm::vec3{2.f, 2.f, 2.f});

	glUseProgram(m_ShaderProgram);
	unsigned int modelLocation = glGetUniformLocation(m_ShaderProgram, "model");
	unsigned int viewLocation = glGetUniformLocation(m_ShaderProgram, "view");
	unsigned int projectionLocation = glGetUniformLocation(m_ShaderProgram, "projection");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetProjection()));

	const glm::vec4 color{1.f, 0.f, 1.f, 1.f};
	unsigned int colorLocation = glGetUniformLocation(m_ShaderProgram, "color");
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));

	glBindVertexArray(m_VertexArray);

	glDrawArrays(GL_TRIANGLES, 0, m_CubeData.NumVertices);
	glCheckError();
}

void AOGL_05_GeometryShading::OnDetach() {
	glDeleteProgram(m_ShaderProgram);
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_MeshVertexBuffer);
}
