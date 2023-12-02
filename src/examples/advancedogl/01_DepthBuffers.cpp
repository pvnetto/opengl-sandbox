#include "01_DepthBuffers.h"

#include "shared/RenderUtils.h"
#include "shared/Utils.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void AOGL_01_DepthBuffers::OnAttach() {
	m_Position = glm::vec3(-0.5f, 0.f, -2.0f);
	m_Scale = glm::vec3(1.2f, 1.2f, 1.2f);
	m_CameraPosition = glm::vec3(0, 0, 0);
	m_FieldOfView = 90.f;

	m_QuadData = Utils::GetQuadData();

	glCreateBuffers(1, &m_QuadVertexBuffer);
	glNamedBufferStorage(m_QuadVertexBuffer, m_QuadData.VerticesSize * sizeof(float), m_QuadData.Vertices, 0);

	glCreateBuffers(1, &m_QuadIndexBuffer);
	glNamedBufferStorage(m_QuadIndexBuffer, m_QuadData.NumIndices * sizeof(unsigned int), m_QuadData.Indices, 0);

	glCreateVertexArrays(1, &m_VertexArray);
	const int vertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_VertexArray, vertexBufferBindingPoint, m_QuadVertexBuffer, NULL, m_QuadData.Layout.getStride());
	for (int i = 0, count = m_QuadData.Layout.getAttributeCount(); i < count; i++) {
		spr::VertexAttribute layoutAttribute;
		if (m_QuadData.Layout.getAttribute(i, layoutAttribute)) {
			glVertexArrayAttribFormat(m_VertexArray, i, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
			glVertexArrayAttribBinding(m_VertexArray, i, vertexBufferBindingPoint);
			glEnableVertexArrayAttrib(m_VertexArray, i);		
		}
	}
	glVertexArrayElementBuffer(m_VertexArray, m_QuadIndexBuffer);

	// 0. Creates FrameBuffer Object
	glCreateFramebuffers(1, &m_Framebuffer);

	const spw::Vec2 windowSize = spw::getWindowSize();
	const int windowWidth = windowSize.X, windowHeight = windowSize.Y;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferTexture);
	glTextureStorage2D(m_ColorBufferTexture, 1, GL_RGB8, windowWidth, windowHeight);

	// 1. Creates Texture Object to use as a Depth buffer
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferTexture);
	glTextureStorage2D(m_DepthBufferTexture, 1, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);

	// 2. Binds depth buffer Texture Object to Framebuffer Object in the depth attachment
	glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0, m_ColorBufferTexture, 0);
	glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_ATTACHMENT, m_DepthBufferTexture, 0);

	// (OPTIONAL) Checks the completeness state of the Framebuffer
	if (glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		assert(false && "::ERROR: Framebuffer is incomplete");
	}

	CreateShaderProgram();

	glClearColor(1.f, 0.f, 1.f, 1.f);
}

void AOGL_01_DepthBuffers::CreateShaderProgram() {
	const std::string vertexSrc = Utils::ReadShaderFile("shaders/01_aogl_depth_buffers.vert");
	const char *vertexSrcC = vertexSrc.c_str();
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

	const std::string fragSrc = Utils::ReadShaderFile("shaders/01_aogl_depth_buffers.frag");
	const char *fragSrcC = fragSrc.c_str();
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

	m_DepthDrawShaderProgram = glCreateProgram();
	glAttachShader(m_DepthDrawShaderProgram, vertexShader);
	glAttachShader(m_DepthDrawShaderProgram, fragmentShader);
	glLinkProgram(m_DepthDrawShaderProgram);

	int programSuccess;
	glGetProgramiv(m_DepthDrawShaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_DepthDrawShaderProgram, 512, NULL, info);
		assert(false && "::ERROR: Failed to link shader program");
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void AOGL_01_DepthBuffers::OnUpdate() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	// 3. Enables depth testing
	glEnable(GL_DEPTH_TEST);
	
	// 4. (OPTIONAL) Sets depth testing parameters. This already comes with sensible defaults, so no need to change anything.
	glDepthMask(GL_TRUE);			// Defaults to GL_TRUE. This value is ANDed with depth values before they're written to the Depth Buffer.
	glDepthFunc(GL_LESS);			// Defaults to GL_LESS. Replaces depth value when it's smaller than the current (i.e., closer to the viewer).

	// 5. Clears values in the Depth Buffer before writing to it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// 6. Renders a bunch of cubes at different depths and draws their depth values
	glm::mat4 view(1.0f);
	view = glm::translate(view, -m_CameraPosition);

	glm::mat4 projection(1.0f);
	const float aspectRatio = (float)(spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f, far = 1000.f;
	projection = glm::perspective(m_FieldOfView, aspectRatio, near, far);

	glUseProgram(m_DepthDrawShaderProgram);

	unsigned int modelLocation = glGetUniformLocation(m_DepthDrawShaderProgram, "model");
	unsigned int viewLocation = glGetUniformLocation(m_DepthDrawShaderProgram, "view");
	unsigned int projectionLocation = glGetUniformLocation(m_DepthDrawShaderProgram, "projection");
	unsigned int nearPlaneLocation = glGetUniformLocation(m_DepthDrawShaderProgram, "nearPlane");
	unsigned int farPlaneLocation = glGetUniformLocation(m_DepthDrawShaderProgram, "farPlane");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1f(nearPlaneLocation, near);
	glUniform1f(farPlaneLocation, far);

	const uint8_t depthRenderTarget = 0;
	const int cubeCount = 10;
	for (int i = 0; i < cubeCount; i++) {
		const float offsetMultiplier = (float) i - cubeCount / 2.f;
		const glm::vec3 offset = offsetMultiplier * glm::vec3{ 0.25f, 0.f, 0.25f };
		glm::mat4 model(1.0f);
		model = glm::translate(model, m_Position + offset);
		model = glm::scale(model, m_Scale);

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_QuadData.NumIndices, GL_UNSIGNED_INT, nullptr);
	}

	// 7. Blits result to default framebuffer, which will get picked up by double buffering and drawn to the screen
	const int defaultFramebuffer = 0;
	const spw::Vec2 resolution = spw::getWindowSize();
	glBlitNamedFramebuffer(m_Framebuffer, defaultFramebuffer,
	                       0, 0, resolution.X, resolution.Y,
	                       0, 0, resolution.X, resolution.Y,
	                       GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
}

void AOGL_01_DepthBuffers::OnDetach() {
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_QuadIndexBuffer);
	glDeleteBuffers(1, &m_QuadVertexBuffer);
	glDeleteProgram(m_DepthDrawShaderProgram);
	glDeleteFramebuffers(1, &m_Framebuffer);
	glDeleteTextures(1, &m_ColorBufferTexture);
	glDeleteTextures(1, &m_DepthBufferTexture);
}
