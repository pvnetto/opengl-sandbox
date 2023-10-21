#include "RendererContextGL.h"

#include "spr/Context.h"
#include "ResourceManager/FramebufferUtilsGL.h"

#include <glad/glad.h>
#include "Helpers.h"

namespace spr {

	void RendererContextGL::init(Context *context, const ContextInfo &info) {
		// TODO: Instead of getting a loader, assume that the user is creating a window without GLFW and do all loading
		// inside spr by using the platform-specific API
		assert(info.GlLoader && "::ERROR: GlLoader was not specified");

		if (bool bGladSuccess = gladLoadGLLoader((GLADloadproc) info.GlLoader); !bGladSuccess) {
			assert(false && "::ERROR: Failed to load GL");
		}

		m_ResourceManager.init(context);

		glCreateVertexArrays(1, &m_DefaultVAO);
		glClearColor(0.1f, 0.1f, 0.12f, 1);			// Sets default clear color

		printf("%s\n", glGetString(GL_VERSION));
	}

	void RendererContextGL::shutdown() {
		glDeleteVertexArrays(1, &m_DefaultVAO);
	}

	void RendererContextGL::render(const FrameData& frameData) {
		glBindVertexArray(m_DefaultVAO);

		processDrawCalls(frameData);
		processBlitRequests(frameData);

		glCheckError();
	}

	void RendererContextGL::processDrawCalls(const FrameData &frameData) {
		DrawCallData cachedDrawCall;
		for (const DrawCallData &drawCall : frameData.DrawCalls) {
			UniformManagerGL &uniformManager = m_ResourceManager.getUniformManager();
			uniformManager.setUniformValues(frameData.UniformDataBuffer, drawCall.UniformsStart, drawCall.UniformsEnd);

			if (const bool bHasRenderTargetChanged = cachedDrawCall.RenderTargetIndex != drawCall.RenderTargetIndex) {
				const RenderTarget &currentRenderTarget = frameData.RenderTargets[cachedDrawCall.RenderTargetIndex];
				const RenderTarget &newRenderTarget = frameData.RenderTargets[drawCall.RenderTargetIndex];

				if (currentRenderTarget.Framebuffer != newRenderTarget.Framebuffer) {
					const FramebufferManagerGL &framebufferManager = m_ResourceManager.getFramebufferManager();
					const FramebufferInstanceGL &framebuffer = framebufferManager.getFramebuffer(newRenderTarget.Framebuffer);
					framebuffer.bind();
				}

				if (currentRenderTarget.ViewportRect != newRenderTarget.ViewportRect) {
					const Rect &newViewportRect = newRenderTarget.ViewportRect;
					glViewport(newViewportRect.OffsetX, newViewportRect.OffsetY, newViewportRect.Width, newViewportRect.Height);
				}

				if (newRenderTarget.ClearFlags != 0) {
					glClear(getClearFlagsGL(newRenderTarget.ClearFlags));
				}

				cachedDrawCall.RenderTargetIndex = drawCall.RenderTargetIndex;
			}

			{
				applyColorState(drawCall.FixedFunctionState.ColorState);
				applyDepthState(drawCall.FixedFunctionState.DepthState);
				applyStencilState(drawCall.FixedFunctionState.StencilState);
				cachedDrawCall.FixedFunctionState = drawCall.FixedFunctionState;
			}

			const ProgramManagerGL &programManager = m_ResourceManager.getProgramManager();
			bool changedAttributesLayout = false;
			if (cachedDrawCall.Program != drawCall.Program) {
				cachedDrawCall.Program = drawCall.Program;
				changedAttributesLayout = true;

				programManager.getProgram(cachedDrawCall.Program).use();
			}

			const ProgramInstanceGL &currentProgram = programManager.getProgram(cachedDrawCall.Program);
			setUniforms(currentProgram.UniformInfoBuffer);

			if (cachedDrawCall.TextureBindings != drawCall.TextureBindings) {
				TextureManagerGL &textureManager = m_ResourceManager.getTextureManager();
				for (uint8_t textureUnit = 0; textureUnit < drawCall.TextureBindings.size(); textureUnit++) {
					const TextureBinding& binding = drawCall.TextureBindings[textureUnit];
					if (!binding.Texture.isValid()) {
						continue;
					}
					
					const bool bIsTextureUnitUnused = textureUnit >= cachedDrawCall.TextureBindings.size();
					const bool bHasTextureChanged = bIsTextureUnitUnused
						|| cachedDrawCall.TextureBindings[textureUnit].Texture != binding.Texture;
					if (bHasTextureChanged) {
						const TextureInstanceGL &texture = textureManager.getTexture(binding.Texture);
						texture.bind(textureUnit);					
					}

					const bool bHasSamplerChanged = bIsTextureUnitUnused
						|| cachedDrawCall.TextureBindings[textureUnit].Sampler.GetHash() != binding.Sampler.GetHash();
					if (bHasSamplerChanged) {
						const SamplerInstanceGL &sampler = textureManager.findOrCreateSampler(binding.Sampler);
						sampler.bind(textureUnit);					
					}
				}

				cachedDrawCall.TextureBindings = drawCall.TextureBindings;
			}

			if (cachedDrawCall.IndexBuffer != drawCall.IndexBuffer) {
				const IndexBufferManagerGL &indexBufferManager = m_ResourceManager.getIndexBufferManager();
				const IndexBufferInstanceGL &indexBuffer = indexBufferManager.getIndexBuffer(drawCall.IndexBuffer);
				indexBuffer.bind(m_DefaultVAO);

				cachedDrawCall.IndexBuffer = drawCall.IndexBuffer;
			}

			const VertexBufferManagerGL &vertexBufferManager = m_ResourceManager.getVertexBufferManager();
			if (cachedDrawCall.VertexBuffer != drawCall.VertexBuffer) {
				changedAttributesLayout = true;

				cachedDrawCall.VertexBuffer = drawCall.VertexBuffer;
			}

			if (changedAttributesLayout) {
				const VertexBufferInstanceGL &vertexBuffer = vertexBufferManager.getVertexBuffer(cachedDrawCall.VertexBuffer);
				currentProgram.bindAttributes(m_DefaultVAO, vertexBuffer);
			}

			if (cachedDrawCall.VertexBuffer.isValid()) {
				if (cachedDrawCall.IndexBuffer.isValid()) {
					const auto &indexBufferManager = m_ResourceManager.getIndexBufferManager();
					const auto &indexBuffer = indexBufferManager.getIndexBuffer(cachedDrawCall.IndexBuffer);
					glDrawElements(GL_TRIANGLES, indexBuffer.IndexCount, GL_UNSIGNED_INT, NULL);
				}
				else {
					const VertexBufferInstanceGL &vertexBuffer = vertexBufferManager.getVertexBuffer(cachedDrawCall.VertexBuffer);
					const auto &vertexAttributeLayout = spr::getVertexAttributeLayout(vertexBuffer.LayoutHandle);
					const int vertexCount = vertexBuffer.Size / vertexAttributeLayout.getStride();
					glDrawArrays(GL_TRIANGLES, 0, vertexCount);
				}			
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RendererContextGL::processBlitRequests(const FrameData &frameData) {
		for (const BlitParameters &blitParams : frameData.BlitRequests) {
			if (blitParams.Source == blitParams.Destination) {
				continue;
			}

			const FramebufferManagerGL &framebufferManager = m_ResourceManager.getFramebufferManager();
			const FramebufferInstanceGL &sourceFramebuffer = framebufferManager.getFramebuffer(blitParams.Source);
			const FramebufferInstanceGL &destinationFramebuffer = framebufferManager.getFramebuffer(blitParams.Destination);
			sourceFramebuffer.blit(destinationFramebuffer, blitParams.SourceRect, blitParams.DestinationRect, blitParams.BlitMask);
		}
	}

	static GLenum getStencilOpGL(const uint8_t opFlag) {
		if (opFlag == spr::StencilBufferState::OpKeep) {
			return GL_KEEP;
		}
		if (opFlag == spr::StencilBufferState::OpZero) {
			return GL_ZERO;
		}
		if (opFlag == spr::StencilBufferState::OpReplace) {
			return GL_REPLACE;
		}
		if (opFlag == spr::StencilBufferState::OpIncr) {
			return GL_INCR;
		}
		if (opFlag == spr::StencilBufferState::OpIncrWrap) {
			return GL_INCR_WRAP;
		}
		if (opFlag == spr::StencilBufferState::OpDecr) {
			return GL_DECR;
		}
		if (opFlag == spr::StencilBufferState::OpDecrWrap) {
			return GL_DECR_WRAP;
		}
		if (opFlag == spr::StencilBufferState::OpInvert) {
			return GL_INVERT;
		}
		assert(false && "spr::ERROR: Unknown stencil op flag");
		return getStencilOpGL(spr::StencilBufferState::OpDefault);
	}

	static GLenum getStencilFnGL(const uint8_t stencilFnFlag) {
		if (stencilFnFlag == spr::StencilBufferState::TestFnNever) {
			return GL_NEVER;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnLess) {
			return GL_LESS;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnEqual) {
			return GL_EQUAL;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnLessOrEqual) {
			return GL_LEQUAL;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnGreater) {
			return GL_GREATER;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnNotEqual) {
			return GL_NOTEQUAL;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnGreaterOrEqual) {
			return GL_GEQUAL;
		}
		if (stencilFnFlag == spr::StencilBufferState::TestFnAlways) {
			return GL_ALWAYS;
		}

		assert(false && "spr::ERROR: Unknown stencil fn flag");
		return getStencilFnGL(spr::StencilBufferState::TestFnDefault);
	}

	static GLenum getDepthFnGL(const uint8_t depthFnFlag) {
		if (depthFnFlag == spr::DepthBufferState::TestFnNever) {
			return GL_NEVER;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnLess) {
			return GL_LESS;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnEqual) {
			return GL_EQUAL;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnLessOrEqual) {
			return GL_LEQUAL;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnGreater) {
			return GL_GREATER;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnNotEqual) {
			return GL_NOTEQUAL;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnGreaterOrEqual) {
			return GL_GEQUAL;
		}
		if (depthFnFlag == spr::DepthBufferState::TestFnAlways) {
			return GL_ALWAYS;
		}

		assert(false && "spr::ERROR: Unknown depth fn flag");
		return getDepthFnGL(spr::DepthBufferState::TestFnDefault);
	}

	static GLenum getBlendFactorGL(const uint8_t factorFlag) {
		if (factorFlag == spr::ColorBufferState::FactorZero)
		{
			return GL_ZERO;
		}
		if (factorFlag == spr::ColorBufferState::FactorOne)
		{
			return GL_ONE;
		}
		if (factorFlag == spr::ColorBufferState::FactorSourceColor)
		{
			return GL_SRC_COLOR;
		}
		if (factorFlag == spr::ColorBufferState::FactorOneMinusSourceColor)
		{
			return GL_ONE_MINUS_SRC_COLOR;
		}
		if (factorFlag == spr::ColorBufferState::FactorDestinationColor)
		{
			return GL_DST_COLOR;
		}
		if (factorFlag == spr::ColorBufferState::FactorOneMinusDestinationColor)
		{
			return GL_ONE_MINUS_DST_COLOR;
		}
		if (factorFlag == spr::ColorBufferState::FactorSourceAlpha)
		{
			return GL_SRC_ALPHA;
		}
		if (factorFlag == spr::ColorBufferState::FactorOneMinusSourceAlpha)
		{
			return GL_ONE_MINUS_SRC_ALPHA;
		}
		if (factorFlag == spr::ColorBufferState::FactorDestinationAlpha)
		{
			return GL_DST_ALPHA;
		}
		if (factorFlag == spr::ColorBufferState::FactorOneMinusDestinationAlpha)
		{
			return GL_ONE_MINUS_DST_ALPHA;
		}
		if (factorFlag == spr::ColorBufferState::FactorConstantColor)
		{
			return GL_CONSTANT_COLOR;
		}
		if (factorFlag == spr::ColorBufferState::FactorOneMinusConstantColor)
		{
			return GL_ONE_MINUS_CONSTANT_COLOR;
		}
		if (factorFlag == spr::ColorBufferState::FactorConstantAlpha)
		{
			return GL_CONSTANT_ALPHA;
		}
		if (factorFlag == spr::ColorBufferState::FactorOneMinusConstantAlpha)
		{
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		}

		assert(false && "spr::ERROR: Unknown factor flag");
		return getBlendFactorGL(spr::ColorBufferState::FactorSourceDefault);
	}

	static GLenum getBlendEquationGL(const uint8_t blendEquationFlag) {
		if (blendEquationFlag == spr::ColorBufferState::BlendEquationAdd) {
			return GL_FUNC_ADD;	
		}
		if (blendEquationFlag == spr::ColorBufferState::BlendEquationSubtract) {
			return GL_FUNC_SUBTRACT;
		}
		if (blendEquationFlag == spr::ColorBufferState::BlendEquationReverseSubtract) {
			return GL_FUNC_REVERSE_SUBTRACT;
		}
		if (blendEquationFlag == spr::ColorBufferState::BlendEquationMinFn) {
			return GL_MIN;
		}
		if (blendEquationFlag == spr::ColorBufferState::BlendEquationMaxFn) {
			return GL_MAX;
		}

		assert(false && "spr::ERROR: Unknown blend equation flag");
		return getBlendEquationGL(spr::ColorBufferState::BlendEquationDefault);
	}

	void RendererContextGL::applyColorState(const spr::ColorBufferState &state) {
		glColorMask(state.RedMask, state.GreenMask, state.BlueMask, state.AlphaMask);

		if (state.bMSAAEnabled) {
			glEnable(GL_MULTISAMPLE);
		}
		else {
			glDisable(GL_MULTISAMPLE);
		}

		if (state.bIsBlendingEnabled) {
			glEnable(GL_BLEND);

			const GLenum blendSourceFactor = getBlendFactorGL(state.BlendingSourceFactor);
			const GLenum blendDestinationFactor = getBlendFactorGL(state.BlendingDestinationFactor);
			glBlendFunc(blendSourceFactor, blendDestinationFactor);

			const GLenum blendEquation = getBlendEquationGL(state.BlendingEquation);
			glBlendEquation(blendEquation);
		}
		else {
			glDisable(GL_BLEND);
		}
	}

	void RendererContextGL::applyDepthState(const spr::DepthBufferState &state) {
		if (state.bIsDepthTestEnabled) {
			glEnable(GL_DEPTH_TEST);
			glDepthMask(state.bDepthWriteMask ? GL_TRUE : GL_FALSE);
			glDepthFunc(getDepthFnGL(state.DepthTestFn));
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RendererContextGL::applyStencilState(const spr::StencilBufferState &state) {
		if (state.bIsStencilTestEnabled) {
			glEnable(GL_STENCIL_TEST);

			const GLenum stencilTestFn = getStencilFnGL(state.StencilTestFn);
			glStencilFunc(stencilTestFn, state.StencilTestReferenceValue, state.StencilTestMask);

			const GLenum stencilFailOp = getStencilOpGL(state.StencilFailOp);
			const GLenum stencilPassDepthFailOp = getStencilOpGL(state.StencilPassDepthFailOp);
			const GLenum stencilPassDepthPassOp = getStencilOpGL(state.StencilPassDepthPassOp);
			glStencilOp(stencilFailOp, stencilPassDepthFailOp, stencilPassDepthPassOp);
		}
		else {
			glDisable(GL_STENCIL_TEST);
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

			// TODO: FIX THIS! It probably doesn't work as intended on arrays!!!
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
			case UniformType::Vec4:
				glUniform4fv(location, 1, uniformManager.getUniformValue<float>(handle, index));
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