#include "RendererContext.h"

#include "shared/renderer/Context.h"
#include "shared/renderer/Programs.h"

#include <glad/glad.h>

namespace spr {
	void RendererContextGL::init() {
		glCreateVertexArrays(1, &m_DefaultVAO);
	}

	void RendererContextGL::shutdown() {
		glDeleteVertexArrays(1, &m_DefaultVAO);
	}

	void RendererContextGL::clear() {
		glClearColor(0.1f, 0.1f, 0.12f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererContextGL::render(const FrameData& frameData) {
		glBindVertexArray(m_DefaultVAO);

		glEnable(GL_DEPTH_TEST);

		DrawCallData currentDrawCall;
		for (const DrawCallData &renderItem : frameData.DrawCalls) {
			auto &uniformManager = m_ResourceManager.getUniformManager();
			uniformManager.setUniformValues(frameData.UniformDataBuffer, renderItem.UniformsStart, renderItem.UniformsEnd);

			bool changedAttributesLayout = false;
			if (currentDrawCall.Program != renderItem.Program) {
				currentDrawCall.Program = renderItem.Program;
				changedAttributesLayout = true;

				spr::useProgram(renderItem.Program);
			}

			setUniforms(getProgramUniforms(renderItem.Program));

			if (currentDrawCall.IndexBuffer != renderItem.IndexBuffer) {
				currentDrawCall.IndexBuffer = renderItem.IndexBuffer;
				const IndexBufferManagerGL& indexBufferManager = m_ResourceManager.getIndexBufferManager();
				const IndexBufferInstanceGL& indexBuffer = indexBufferManager.getIndexBuffer(currentDrawCall.IndexBuffer);
				indexBuffer.bind(m_DefaultVAO);
			}

			const VertexBufferManagerGL& vertexBufferManager = m_ResourceManager.getVertexBufferManager();
			if (currentDrawCall.VertexBuffer != renderItem.VertexBuffer) {
				currentDrawCall.VertexBuffer = renderItem.VertexBuffer;
				changedAttributesLayout = true;

				const VertexBufferInstanceGL& vertexBuffer = vertexBufferManager.getVertexBuffer(currentDrawCall.VertexBuffer);
				vertexBuffer.bind();
			}

			if (changedAttributesLayout) {
				const VertexBufferInstanceGL& vertexBuffer = vertexBufferManager.getVertexBuffer(currentDrawCall.VertexBuffer);
				spr::internal::bindVertexAttributeLayout(currentDrawCall.Program, vertexBuffer.LayoutHandle);
			}

			if (currentDrawCall.IndexBuffer.isValid()) {
				const auto &indexBufferManager = m_ResourceManager.getIndexBufferManager();
				const auto &indexBuffer = indexBufferManager.getIndexBuffer(currentDrawCall.IndexBuffer);
				glDrawElements(GL_TRIANGLES, indexBuffer.IndexCount, GL_UNSIGNED_INT, NULL);
			}
			else {
				const VertexBufferInstanceGL& vertexBuffer = vertexBufferManager.getVertexBuffer(currentDrawCall.VertexBuffer);
				const auto &vertexAttributeLayout = spr::getVertexAttributeLayout(vertexBuffer.LayoutHandle);
				const int vertexCount = vertexBuffer.Size / vertexAttributeLayout.getStride();
				glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			}
		}
	}

	void RendererContextGL::setUniforms(UniformInfoBufferPtr uniformInfoBuffer) {
		auto &uniformManager = m_ResourceManager.getUniformManager();

		// Assumes buffer is at end location
		// TODO: Read all uniforms at once so we don't have to rely on the buffer being at the right state
		uint32_t bufferEnd = uniformInfoBuffer->getPos();
		uniformInfoBuffer->reset();
		while (uniformInfoBuffer->getPos() < bufferEnd) {
			UniformType type = uniformInfoBuffer->read<UniformType>();
			uint32_t location = uniformInfoBuffer->read<uint32_t>();
			UniformHandle handle = uniformInfoBuffer->read<UniformHandle>();
			switch (type) {
			case UniformType::Float:
				glUniform1fv(location, 1, uniformManager.getUniformValue<float>(handle));
				break;
			case UniformType::Sampler:
			case UniformType::Integer:
				glUniform1iv(location, 1, uniformManager.getUniformValue<int>(handle));
				break;
			case UniformType::Vec2:
				glUniform2fv(location, 1, uniformManager.getUniformValue<float>(handle));
				break;
			case UniformType::Vec3:
				glUniform3fv(location, 1, uniformManager.getUniformValue<float>(handle));
				break;
			default:
				assert(false && "::ERROR: Undefined uniform type");
				break;
			}
		}
	}
}