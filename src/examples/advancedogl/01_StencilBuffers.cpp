#include "01_StencilBuffers.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"
#include "shared/Utils.h"
#include "shared/GlUtils.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void AOGL_01_StencilBuffers::OnAttach() {
	m_Camera = Camera(
	    glm::vec3(-2.8f, 2.f, 2.8f),
	    glm::vec3(-26.f, -42.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_ObjectColor = glm::vec3{0.2f, 0.35f, 1.f};
	m_LightColor = glm::vec3{0.8f, 0.8f, 1.f};


	{
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

	{
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

	m_LightingShaderProgram = Utils::CreateShaderProgramGL("shaders/07_phong.vert", "shaders/07_phong.frag");
	m_LightSourceShaderProgram = Utils::CreateShaderProgramGL("shaders/05_vertex_mvp.vert", "shaders/06_frag_light_source.frag");
	m_StencilPassShaderProgram = Utils::CreateShaderProgramGL("shaders/01_aogl_stencil_buffers_sdf.vert", "shaders/01_aogl_stencil_buffers_sdf.frag");

	glCreateFramebuffers(1, &m_Framebuffer);

	const spw::Vec2 windowSize = spw::getWindowSize();
	const int windowWidth = windowSize.X, windowHeight = windowSize.Y;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferTexture);
	glTextureStorage2D(m_ColorBufferTexture, 1, GL_RGB8, windowWidth, windowHeight);

	// 0. Creates Texture Object to use as both a Stencil Buffer and Depth Buffer
	glCreateTextures(GL_TEXTURE_2D, 1, &m_StencilDepthBufferTexture);
	glTextureStorage2D(m_StencilDepthBufferTexture, 1, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);

	// 1. Attaches Texture Objects to Framebuffer Object, including the Stencil Texture we just created
	glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0, m_ColorBufferTexture, 0);
	glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, m_StencilDepthBufferTexture, 0);

	// (OPTIONAL) Checks the completeness state of the Framebuffer
	if (auto status = glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE) {
		assert(false && "::ERROR: Framebuffer is incomplete");
	}

	// 2. Enables stencil testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glClearColor(0.88f, 0.88f, 0.88f, 1.0f);
}

void AOGL_01_StencilBuffers::OnUpdate() {
	/* Stencil Pass */
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

		// 3. Sets stencil testing parameters
		glStencilFunc(GL_ALWAYS, 1, 0xff);					// Sets Stencil Value to 1, comparison function is set to ALWAYS so all fragments pass the test and are written to the buffer.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xff);								// Determines which bits of the Stencil Buffer we're allowed to write to.
	
		// (OPTIONAL) Disables writing to the Color Buffer. We do this because the purpose of the first pass is to setup the stencil buffer.
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		// 4. Clears Stencil Buffer before writing to it. This will reset the Stencil Buffer to all 0s.
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		// 0 0 0 0 0 0 0 0
		glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 5. Draws quad with circle hole to the screen to fill the stencil buffer with 1s
		// At this point, the Stencil Buffer will look something like this:
		// 1 1 1 1 1 1 1 1
		// 1 1 1 1 1 1 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 0 0 0 0 1 1
		// 1 1 1 1 1 1 1 1
		// 1 1 1 1 1 1 1 1
		glUseProgram(m_StencilPassShaderProgram);

		const spw::Vec2 windowSize = spw::getWindowSize();
		const int resolutionLocation = glGetUniformLocation(m_StencilPassShaderProgram, "inResolution");
		glUniform2f(resolutionLocation, (float)windowSize.X, (float) windowSize.Y);

		const float currentTime = Runtime::get()->getTime();
		const int timeLocation = glGetUniformLocation(m_StencilPassShaderProgram, "inTime");
		glUniform1f(timeLocation, currentTime);

		glBindVertexArray(m_QuadVertexArray);
		glDrawElements(GL_TRIANGLES, m_QuadData.NumIndices, GL_UNSIGNED_INT, nullptr);
	}

	/* Render Pass */
	{
		// 6. Configures the Render Pass to use the Stencil Buffer as a difference mask (i.e., don't draw where the mask is 1)
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);				// Sets comparison function to NOTEQUAL, so that we only draw in places where the Stencil not 1 (or 0, pretty much a subtract mask).
		glStencilMask(0x00);								// Disables writing to the Stencil Buffer.

		// (OPTIONAL) Reenables writing to the Color Buffer.
		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glm::vec3 lightSourcePos = glm::vec3(std::cos(Runtime::get()->getTime()),
		                                     std::cos(Runtime::get()->getTime()) * std::sin(Runtime::get()->getTime()),
		                                     std::sin(Runtime::get()->getTime()) * 2.f);

		glm::mat4 sourceModel(1.0f);
		sourceModel = glm::translate(sourceModel, lightSourcePos);
		sourceModel = glm::scale(sourceModel, glm::vec3(0.3f, 0.3f, 0.3f));

		glUseProgram(m_LightSourceShaderProgram);
		{
			unsigned int modelLocation = glGetUniformLocation(m_LightSourceShaderProgram, "model");
			unsigned int viewLocation = glGetUniformLocation(m_LightSourceShaderProgram, "view");
			unsigned int projectionLocation = glGetUniformLocation(m_LightSourceShaderProgram, "projection");
			unsigned int lightColorLocation = glGetUniformLocation(m_LightSourceShaderProgram, "lightColor");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sourceModel));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetView()));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetProjection()));
			glUniform3fv(lightColorLocation, 1, glm::value_ptr(m_LightColor));
		}

		glBindVertexArray(m_CubeVertexArray);
		glDrawArrays(GL_TRIANGLES, 0, m_CubeData.NumVertices);

		glm::mat4 litModel(1.0f);
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
		litModel = glm::rotate(litModel, Runtime::get()->getTime() * 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));

		glUseProgram(m_LightingShaderProgram);
		{
			unsigned int modelLocation = glGetUniformLocation(m_LightingShaderProgram, "model");
			unsigned int viewLocation = glGetUniformLocation(m_LightingShaderProgram, "view");
			unsigned int projectionLocation = glGetUniformLocation(m_LightingShaderProgram, "projection");
			unsigned int lightColorLocation = glGetUniformLocation(m_LightingShaderProgram, "lightColor");
			unsigned int lightPosLocation = glGetUniformLocation(m_LightingShaderProgram, "lightPos");
			unsigned int viewPosLocation = glGetUniformLocation(m_LightingShaderProgram, "viewPos");
			unsigned int objectColorLocation = glGetUniformLocation(m_LightingShaderProgram, "objectColor");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(litModel));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetView()));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_Camera.GetProjection()));
			glUniform3fv(lightColorLocation, 1, glm::value_ptr(m_LightColor));
			glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightSourcePos));
			glUniform3fv(viewPosLocation, 1, glm::value_ptr(m_Camera.GetPosition()));
			glUniform3fv(objectColorLocation, 1, glm::value_ptr(m_ObjectColor));
		}

		glBindVertexArray(m_CubeVertexArray);
		glDrawArrays(GL_TRIANGLES, 0, m_CubeData.NumVertices);
	}

	/* Draw Pass */
	// 7. Blits result to default framebuffer, which will get picked up by double buffering and drawn to the screen
	{
		const int defaultFramebuffer = 0;
		const spw::Vec2 resolution = spw::getWindowSize();
		glBlitNamedFramebuffer(m_Framebuffer, defaultFramebuffer,
							   0, 0, resolution.X, resolution.Y,
							   0, 0, resolution.X, resolution.Y,
							   GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
	}
}

void AOGL_01_StencilBuffers::OnDetach() {
	glDeleteVertexArrays(1, &m_QuadVertexArray);
	glDeleteBuffers(1, &m_QuadIndexBuffer);
	glDeleteBuffers(1, &m_QuadVertexBuffer);

	glDeleteVertexArrays(1, &m_CubeVertexArray);
	glDeleteBuffers(1, &m_CubeVertexBuffer);

	glDeleteProgram(m_LightingShaderProgram);
	glDeleteProgram(m_LightSourceShaderProgram);
	glDeleteProgram(m_StencilPassShaderProgram);

	glDeleteFramebuffers(1, &m_Framebuffer);
	glDeleteTextures(1, &m_ColorBufferTexture);
	glDeleteTextures(1, &m_StencilDepthBufferTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
