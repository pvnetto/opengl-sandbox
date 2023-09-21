#include "04_Instancing.h"

#include "shared/renderer/OpenGL/Helpers.h"
#include "shared/RenderUtils.h"
#include "shared/Utils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include <iostream>

static std::vector<float> GetColors(int amount) {
	std::vector<float> colors;
	for (int i = 0; i < amount; i++) {
		const float progress = i / (float) amount;
		const float r = glm::mix(0.f, 1.f, progress);
		const float g = 0.f;
		const float b = 0.f/*glm::mix(1.f, 0.f, progress)*/;
		const float a = 1.f;
		colors.push_back(r);
		colors.push_back(g);
		colors.push_back(b);
		colors.push_back(a);
	}
	return colors;
}

static std::vector<float> GetWorldPositions(int amount) {
	const int rowSize = sqrt(amount);
	const float gridSize = 2.f;
	std::vector<float> worldPositions;
	for (int cubeIndex = 0; cubeIndex < amount; cubeIndex++) {
		const int rowIdx = (cubeIndex / rowSize) - (rowSize / 2);
		const int colIdx = (cubeIndex % rowSize) - (rowSize / 2);

		const glm::vec3 worldPosition { rowIdx * gridSize, colIdx * gridSize, -10.f };
		for (int positionIndex = 0; positionIndex < 3; positionIndex++) {
			worldPositions.push_back(worldPosition[positionIndex]);
		}
	}

	return worldPositions;
}

void AOGL_04_Instancing::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));

	const glm::vec2 windowSize = spr::getWindowSize();
	const float aspectRatio = windowSize.x / windowSize.y;
	m_Camera.SetPerspective(90.f, aspectRatio, 0.1f, 100.f);

	m_CubeData = Utils::GetCubeData();

	// 0. Creates Vertex Buffer to store per-vertex data (vertices, normals, UVs etc)
	glCreateBuffers(1, &m_MeshVertexBuffer);
	glNamedBufferStorage(m_MeshVertexBuffer, m_CubeData.VerticesSize * sizeof(float), m_CubeData.Vertices, 0);

	// 1. Creates Vertex Buffer to store per-instance color data
	const std::vector<float> colors = GetColors(m_CubeCount);
	glCreateBuffers(1, &m_PerInstanceColorVertexBuffer);
	glNamedBufferStorage(m_PerInstanceColorVertexBuffer, colors.size() * sizeof(float), colors.data(), 0);

	// 3. Creates Vertex Buffer to store per-instance world positions
	// NOTE: We could have used the same Vertex Buffer that we use for colors, but we created a separate one to make it easier to use different divisors
	const std::vector<float> worldPositions = GetWorldPositions(m_CubeCount);
	glCreateBuffers(1, &m_PerInstancePositionVertexBuffer);
	glNamedBufferStorage(m_PerInstancePositionVertexBuffer, worldPositions.size() * sizeof(float), worldPositions.data(), 0);

	// 4. Creates Vertex Array to store both per-vertex and per-instance layout attributes
	glCreateVertexArrays(1, &m_VertexArray);

	// 5. Defines attribute layout for Vertex Buffer with per-vertex data
	const int perVertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_VertexArray, perVertexBufferBindingPoint, m_MeshVertexBuffer, NULL, m_CubeData.Layout.getStride());
	for (int i = 0, count = m_CubeData.Layout.getAttributeCount(); i < count; i++) {
		const spr::VertexAttribute &layoutAttribute = m_CubeData.Layout.getAttribute(i);

		glVertexArrayAttribFormat(m_VertexArray, i, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
		glVertexArrayAttribBinding(m_VertexArray, i, perVertexBufferBindingPoint);
		
		glEnableVertexArrayAttrib(m_VertexArray, i);
	}

	
	// 6. Defines attribute layout for Vertex Buffer with per-instance color data
	// NOTE: This is almost exactly the same as the other one, but we specify a divisor
	const int baseAttributeIndex = m_CubeData.Layout.getAttributeCount();				// Takes the next available attribute index
	{
		const int colorBufferBindingPoint = 1;
		const int colorStride = 4 * sizeof(float);
		glVertexArrayVertexBuffer(m_VertexArray, colorBufferBindingPoint, m_PerInstanceColorVertexBuffer, NULL, colorStride);

		glVertexArrayAttribFormat(m_VertexArray, baseAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_VertexArray, baseAttributeIndex, colorBufferBindingPoint);
		glVertexArrayBindingDivisor(m_VertexArray, colorBufferBindingPoint, 1);				// Tells Input Assembly to issue a new color value every 2 instances

		glEnableVertexArrayAttrib(m_VertexArray, baseAttributeIndex);
	}


	// 7. Defines attribute layout for Vertex Buffer with per-instance world positions
	// NOTE: Ideally we would pass the entire model matrix here, but there's a limitation for attributes that they cannot contain 16 values at once, so we'd
	// have to create 4 attributes to pass a mat4 matrix. This seemed too cumbersome for me, so I'm passing just the world position and calculating the model
	// matrix inside the shader.
	{
		const int attributeIndex = baseAttributeIndex + 1;
		const int positionBufferBindingPoint = 2;
		const int positionStride = 3 * sizeof(float);
		glVertexArrayVertexBuffer(m_VertexArray, positionBufferBindingPoint, m_PerInstancePositionVertexBuffer, NULL, positionStride);
	
		glVertexArrayAttribFormat(m_VertexArray, attributeIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(m_VertexArray, attributeIndex);
		glVertexArrayAttribBinding(m_VertexArray, attributeIndex, positionBufferBindingPoint);
		glVertexArrayBindingDivisor(m_VertexArray, positionBufferBindingPoint, 1);			// Tells Input Assembly to issue a new position value every instance
	}


	// 8. Defines per-instance attributes in the Vertex Shader, these look exactly the same as per-vertex attributes
	CreateShaderProgram();

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}

void AOGL_04_Instancing::CreateShaderProgram() {
	const std::string vertexSrc = Utils::ReadShaderFile("shaders/04_aogl_instanced.vert");
	const char* vertexSrcC = vertexSrc.c_str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

	const std::string fragSrc = Utils::ReadShaderFile("shaders/04_aogl_instanced.frag");
	const char* fragSrcC = fragSrc.c_str();
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);

	int programSuccess;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, info);
		assert(false && "::ERROR: Failed to link shader program");
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void AOGL_04_Instancing::OnUpdate() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glUseProgram(m_ShaderProgram);
	unsigned int viewLocation = glGetUniformLocation(m_ShaderProgram, "view");
	unsigned int projectionLocation = glGetUniformLocation(m_ShaderProgram, "projection");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetProjection()));

	// 9. Binds VAO with per-vertex/per-instance attributes
	glBindVertexArray(m_VertexArray);

	// 10. Calls glDraw{*}Instanced method with the number of instances
	// NOTE: This is essentially the same as calling the glDraw{*} non-instanced version 'n' times, except it's more performant
	glDrawArraysInstanced(GL_TRIANGLES, 0, m_CubeData.NumVertices, m_CubeCount);
	glCheckError();
}

void AOGL_04_Instancing::OnDetach() {
	glDeleteProgram(m_ShaderProgram);
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_MeshVertexBuffer);
	glDeleteBuffers(1, &m_PerInstanceColorVertexBuffer);
	glDeleteBuffers(1, &m_PerInstancePositionVertexBuffer);
}
