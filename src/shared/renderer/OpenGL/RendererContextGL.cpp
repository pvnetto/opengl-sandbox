#include "RendererContextGL.h"

#include "shared/renderer/Context.h"

#include <glad/glad.h>

namespace spr {

	void RendererContextGL::init(Context* context) {
		m_ResourceManager.init(context);
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

		DrawCallData cachedDrawCall;
		for (const DrawCallData &drawCall : frameData.DrawCalls) {
			UniformManagerGL &uniformManager = m_ResourceManager.getUniformManager();
			uniformManager.setUniformValues(frameData.UniformDataBuffer, drawCall.UniformsStart, drawCall.UniformsEnd);

			const ProgramManagerGL& programManager = m_ResourceManager.getProgramManager();
			bool changedAttributesLayout = false;
			if (cachedDrawCall.Program != drawCall.Program) {
				cachedDrawCall.Program = drawCall.Program;
				changedAttributesLayout = true;

				programManager.getProgram(cachedDrawCall.Program).use();
			}

			const ProgramInstanceGL &currentProgram = programManager.getProgram(cachedDrawCall.Program);
			setUniforms(currentProgram.UniformInfoBuffer);

			// TODO: We could probably cache these as well, and avoid binding samplers every frame
			TextureManagerGL &textureManager = m_ResourceManager.getTextureManager();
			for (const auto &[textureUnit, textureBinding] : drawCall.TextureBindings) {
				const TextureInstanceGL& texture = textureManager.getTexture(textureBinding.Texture);
				texture.bind(textureUnit);

				const SamplerInstanceGL &sampler = textureManager.findOrCreateSampler(textureBinding.Sampler);
				sampler.bind(textureUnit);
			}

			if (cachedDrawCall.IndexBuffer != drawCall.IndexBuffer) {
				cachedDrawCall.IndexBuffer = drawCall.IndexBuffer;
				const IndexBufferManagerGL& indexBufferManager = m_ResourceManager.getIndexBufferManager();
				const IndexBufferInstanceGL& indexBuffer = indexBufferManager.getIndexBuffer(cachedDrawCall.IndexBuffer);
				indexBuffer.bind(m_DefaultVAO);
			}

			const VertexBufferManagerGL& vertexBufferManager = m_ResourceManager.getVertexBufferManager();
			if (cachedDrawCall.VertexBuffer != drawCall.VertexBuffer) {
				cachedDrawCall.VertexBuffer = drawCall.VertexBuffer;
				changedAttributesLayout = true;

				const VertexBufferInstanceGL& vertexBuffer = vertexBufferManager.getVertexBuffer(cachedDrawCall.VertexBuffer);
				vertexBuffer.bind();
			}

			if (changedAttributesLayout) {
				const VertexBufferInstanceGL& vertexBuffer = vertexBufferManager.getVertexBuffer(cachedDrawCall.VertexBuffer);
				currentProgram.bindAttributes(m_DefaultVAO, vertexBuffer);
			}

			if (cachedDrawCall.IndexBuffer.isValid()) {
				const auto &indexBufferManager = m_ResourceManager.getIndexBufferManager();
				const auto &indexBuffer = indexBufferManager.getIndexBuffer(cachedDrawCall.IndexBuffer);
				glDrawElements(GL_TRIANGLES, indexBuffer.IndexCount, GL_UNSIGNED_INT, NULL);
			}
			else {
				const VertexBufferInstanceGL& vertexBuffer = vertexBufferManager.getVertexBuffer(cachedDrawCall.VertexBuffer);
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
			ProgramUniformInfoGL uniformInfo = uniformInfoBuffer->read<ProgramUniformInfoGL>();
			const auto& [location, handle, index, type] = uniformInfo;

			switch (type) {
			case UniformType::Float:
				glUniform1fv(location, 1, uniformManager.getUniformValue<float>(handle, index));
				break;
			case UniformType::Sampler:
			case UniformType::Integer:
				glUniform1iv(location, 1, uniformManager.getUniformValue<int>(handle, index));
				break;
			case UniformType::Vec2:
				glUniform2fv(location, 1, uniformManager.getUniformValue<float>(handle, index));
				break;
			case UniformType::Vec3:
				glUniform3fv(location, 1, uniformManager.getUniformValue<float>(handle, index));
				break;
			case UniformType::Mat4x4:
				glUniformMatrix4fv(location, 1, GL_FALSE, uniformManager.getUniformValue<float>(handle, index));
				break;
			default:
				assert(false && "::ERROR: Undefined uniform type");
				break;
			}
		}
	}
}