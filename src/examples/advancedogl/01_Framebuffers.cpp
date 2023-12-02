#include "01_Framebuffers.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"
#include "shared/Utils.h"
#include "shared/GlUtils.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

void AOGL_01_Framebuffers::OnAttach() {
	m_Position = glm::vec3(-0.2f, 0.3f, -2.0f);
	m_Scale = glm::vec3(1.2f, 1.2f, 1.2f);
	m_CameraPosition = glm::vec3(0, 0, 0);
	m_FieldOfView = 90.f;

	CreateQuad();
	CreateCube();

	m_BrickTexture = Utils::CreateTextureGL("assets/bricks.jpg");
	m_LadybugTexture = Utils::CreateTextureGL("assets/yps.png");

	m_DefaultShaderProgram = Utils::CreateShaderProgramGL("shaders/05_vertex_mvp.vert", "shaders/03_frag_tex.frag");
	m_PostProcessShaderProgram = Utils::CreateShaderProgramGL("shaders/01_aogl_color_buffers.vert", "shaders/01_aogl_color_buffers.frag");

	// 0. Creates FrameBuffer Object
	glCreateFramebuffers(1, &m_Framebuffer);

	// 1. Creates Texture Object to use as a Color Buffer
	const spw::Vec2 windowSize = spw::getWindowSize();
	const int windowWidth = windowSize.X, windowHeight = windowSize.Y;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferTexture);
	glTextureStorage2D(m_ColorBufferTexture, 1, GL_RGB8, windowWidth, windowHeight);

	// 2. Creates Texture Object to use as a Depth buffer (more on this later)
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferTexture);
	glTextureStorage2D(m_DepthBufferTexture, 1, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);

	// 3. Binds Texture Object to Framebuffer Object in one of the color attachments
	glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0, m_ColorBufferTexture, 0);
	glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_ATTACHMENT, m_DepthBufferTexture, 0);

	// (OPTIONAL) Checks the completeness state of the Framebuffer
	if (glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		assert(false && "::ERROR: Framebuffer is incomplete");
	}
}

void AOGL_01_Framebuffers::OnUpdate() {
	// 4. Binds Framebuffer Object to GL context
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	// 5. Performs a render pass on the Framebuffer
	DrawScene();

	// 6. Binds default Framebuffer (this is the one provided by the windowing system)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 7. Gets the Framebuffer Object color attachment and draws it in another render pass
	const int renderTargetTextureUnit = 2;
	glBindTextureUnit(renderTargetTextureUnit, m_ColorBufferTexture);

	glUseProgram(m_PostProcessShaderProgram);
	int renderTargetTextureLocation = glGetUniformLocation(m_PostProcessShaderProgram, "renderTargetTexture");
	glUniform1i(renderTargetTextureLocation, renderTargetTextureUnit);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_QuadVertexArray);
	glDrawElements(GL_TRIANGLES, m_QuadData.NumIndices, GL_UNSIGNED_INT, nullptr);
}

void AOGL_01_Framebuffers::DrawScene() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, m_Position);
	model = glm::rotate(model, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, m_Scale);

	glm::mat4 view(1.0f);
	view = glm::translate(view, -m_CameraPosition);

	glm::mat4 projection(1.0f);
	const float aspectRatio = (float)(spw::getWindowWidth() / spw::getWindowHeight());
	const float near = 0.1f;
	const float far = 100.f;
	projection = glm::perspective(m_FieldOfView, aspectRatio, near, far);

	glUseProgram(m_DefaultShaderProgram);

	const int texUnit = 0, anotherTexUnit = 1;
	glBindTextureUnit(texUnit, m_BrickTexture);
	glBindTextureUnit(anotherTexUnit, m_LadybugTexture);

	unsigned int modelLocation = glGetUniformLocation(m_DefaultShaderProgram, "model");
	unsigned int viewLocation = glGetUniformLocation(m_DefaultShaderProgram, "view");
	unsigned int projectionLocation = glGetUniformLocation(m_DefaultShaderProgram, "projection");
	unsigned int texLocation = glGetUniformLocation(m_DefaultShaderProgram, "tex");
	unsigned int anotherTexLocation = glGetUniformLocation(m_DefaultShaderProgram, "anotherTex");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(texLocation, texUnit);
	glUniform1i(anotherTexLocation, anotherTexUnit);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_CubeVertexArray);
	glDrawArrays(GL_TRIANGLES, 0, m_CubeData.NumVertices);
}

void AOGL_01_Framebuffers::OnDetach() {
	glDeleteVertexArrays(1, &m_QuadVertexArray);
	glDeleteBuffers(1, &m_QuadVertexBuffer);
	glDeleteBuffers(1, &m_QuadIndexBuffer);

	glDeleteVertexArrays(1, &m_CubeVertexArray);
	glDeleteBuffers(1, &m_CubeVertexBuffer);

	glDeleteTextures(1, &m_BrickTexture);
	glDeleteTextures(1, &m_LadybugTexture);

	glDeleteProgram(m_DefaultShaderProgram);
	glDeleteProgram(m_PostProcessShaderProgram);

	glDeleteFramebuffers(1, &m_Framebuffer);
	glDeleteTextures(1, &m_ColorBufferTexture);
	glDeleteTextures(1, &m_DepthBufferTexture);
}

void AOGL_01_Framebuffers::CreateQuad() {
	m_QuadData = Utils::GetScreenQuadData();

	glCreateBuffers(1, &m_QuadVertexBuffer);
	glNamedBufferStorage(m_QuadVertexBuffer, m_QuadData.VerticesSize * sizeof(float), m_QuadData.Vertices, 0);

	glCreateBuffers(1, &m_QuadIndexBuffer);
	glNamedBufferStorage(m_QuadIndexBuffer, m_QuadData.NumIndices * sizeof(unsigned int), m_QuadData.Indices, 0);

	glCreateVertexArrays(1, &m_QuadVertexArray);
	const int vertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_QuadVertexArray, vertexBufferBindingPoint, m_QuadVertexBuffer, NULL, m_QuadData.Layout.getStride());
	for (int i = 0, count = m_QuadData.Layout.getAttributeCount(); i < count; i++) {
		spr::VertexAttribute layoutAttribute;
		if (m_QuadData.Layout.getAttribute(i, layoutAttribute)) {
			glVertexArrayAttribFormat(m_QuadVertexArray, i, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
			glVertexArrayAttribBinding(m_QuadVertexArray, i, vertexBufferBindingPoint);
			glEnableVertexArrayAttrib(m_QuadVertexArray, i);
		}
	}
	glVertexArrayElementBuffer(m_QuadVertexArray, m_QuadIndexBuffer);
}

void AOGL_01_Framebuffers::CreateCube() {
	m_CubeData = Utils::GetCubeData();

	glCreateBuffers(1, &m_CubeVertexBuffer);
	glNamedBufferStorage(m_CubeVertexBuffer, m_CubeData.VerticesSize * sizeof(float), m_CubeData.Vertices, 0);

	glCreateVertexArrays(1, &m_CubeVertexArray);

	const int perVertexBufferBindingPoint = 0;
	glVertexArrayVertexBuffer(m_CubeVertexArray, perVertexBufferBindingPoint, m_CubeVertexBuffer, NULL, m_CubeData.Layout.getStride());
	for (int i = 0, count = m_CubeData.Layout.getAttributeCount(); i < count; i++) {
		spr::VertexAttribute layoutAttribute;
		if (m_CubeData.Layout.getAttribute(i, layoutAttribute)) {
			glVertexArrayAttribFormat(m_CubeVertexArray, i, layoutAttribute.Num, GL_FLOAT, GL_FALSE, layoutAttribute.Offset);
			glVertexArrayAttribBinding(m_CubeVertexArray, i, perVertexBufferBindingPoint);

			glEnableVertexArrayAttrib(m_CubeVertexArray, i);		
		}
	}
}
