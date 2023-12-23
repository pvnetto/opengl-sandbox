#include "FrameDataManager.h"

#include "spr/Context.h"
#include "spr/ResourceManager/SamplerInfo.h"
#include "spr/Utils.h"

#include <cassert>
#include <algorithm>

#define CHECK_RENDER_TARGET_INDEX(index) assert(index >= 0 && index < RenderTarget::kMaxRenderTargets)

namespace spr {

	static const uint8_t k_DefaultRenderTargetIndex = 0;

	void FrameData::clear() {
		UniformDataBuffer->reset();
		DrawCalls.clear();
		BlitRequests.clear();
		SortMode = DrawCallSortMode::Default;

		for (uint8_t i = 0; i < RenderTarget::kMaxRenderTargets; i++) {
			RenderTargets[i].Framebuffer = k_InvalidHandle;
		}
	}

	// DrawCalls are sorted by descending order. If DrawCallA > DrawCallB, it should be executed first.
	static void sortDrawCallsByState(std::vector<DrawCallData>& drawCalls) {
		std::sort(drawCalls.begin(), drawCalls.end(), [](const auto &drawCallA, const auto &drawCallB) {
			return drawCallA > drawCallB;
		});
	}

	void FrameData::sortDrawCalls() {
		switch (SortMode) {
		case spr::DrawCallSortMode::Default:
			sortDrawCallsByState(DrawCalls);
			break;
		case spr::DrawCallSortMode::OrderDependent:
			// Does nothing, uses submit order
			break;
		default:
			assert(false && "spr::ERROR: Draw call sort mode is not supported yet");
			break;
		}
	}

	uint32_t FrameData::lastDrawCallUniformEnd() {
		if (DrawCalls.empty()) {
			return 0;
		}
		return DrawCalls[DrawCalls.size() - 1].UniformsEnd;
	}

	void DrawCallData::clear() {
		FixedFunctionState = {};

		VertexBuffers.clear();
		IndexBuffer = k_InvalidHandle;

		Program = k_InvalidHandle;
		UniformsStart = 0;
		UniformsEnd = 0;
		TextureBindings.clear();
	}

	// Not so great for ordering as it's basically an equality check, but still better than nothing
	static std::size_t getTextureStateHash(const spr::TextureBindings &textureBindings) {
		std::size_t hash = 0;

		for (uint8_t textureUnit = 0; textureUnit < textureBindings.size(); textureUnit++) {
			const TextureBinding &textureBinding = textureBindings[textureUnit]; 
			spr::hashCombine(hash,
					textureBinding.Sampler.GetHash(),
					murmurHash2A(&textureBinding.Texture.idx, sizeof(uint8_t)),
					murmurHash2A(&textureUnit, sizeof(uint8_t)));
		}

		return hash;
	}

	DrawCallSortKey DrawCallData::getSortKey() const {
		DrawCallSortKey sortKey;
		sortKey.RenderTargetIndexInversed = (RenderTargetIndex ^ 0b11111111);
		sortKey.bIsBlendingDisabled = !(!!FixedFunctionState.ColorState.bIsBlendingEnabled);
		sortKey.BlendingEquation = FixedFunctionState.ColorState.BlendingEquation;
		sortKey.ShaderProgram = Program.idx;
		sortKey.TextureStateHash = getTextureStateHash(TextureBindings);
		return sortKey;
	}

	void FrameDataManager::init(Context *owner, const struct ContextInfo &info) {
		m_Owner = owner;
		m_CurrentFrame.UniformDataBuffer = UniformDataBuffer::alloc();
		m_CurrentFrame.RenderTargets[k_DefaultRenderTargetIndex].ViewportRect = { 0, 0, info.Width, info.Height }; 
	}

	void FrameDataManager::submit(uint8_t renderTargetIndex, ProgramHandle &program) {
		CHECK_RENDER_TARGET_INDEX(renderTargetIndex);

		m_CurrentDrawCall.RenderTargetIndex = renderTargetIndex;
		m_CurrentDrawCall.Program = program;
		m_CurrentDrawCall.UniformsStart = m_CurrentFrame.lastDrawCallUniformEnd();
		m_CurrentDrawCall.UniformsEnd = m_CurrentFrame.UniformDataBuffer->getPos();
		m_CurrentFrame.DrawCalls.push_back(m_CurrentDrawCall);

		m_CurrentDrawCall.clear();
	}

	void FrameDataManager::reset() {
		m_CurrentFrame.clear();
		m_CurrentDrawCall.clear();
	}

	void FrameDataManager::sortDrawCalls() {
		m_CurrentFrame.sortDrawCalls();
	}

	void FrameDataManager::setDrawCallSortMode(DrawCallSortMode sortMode) {
		m_CurrentFrame.SortMode = sortMode;
	}

	void FrameDataManager::setVertexBuffer(const VertexBufferHandle handle) {
		m_CurrentDrawCall.VertexBuffers.push_back(handle);
	}

	void FrameDataManager::setIndexBuffer(const IndexBufferHandle handle) {
		assert(!m_CurrentDrawCall.IndexBuffer.isValid() && "::ERROR: Index buffer was already set in this draw call");
		m_CurrentDrawCall.IndexBuffer = handle;
	}

	void FrameDataManager::setUniform(const UniformHandle handle, const void *data, uint8_t count) {
		assert(handle.isValid() && "::ERROR: Invalid uniform handle!");
		const UniformManager& uniformManager = m_Owner->getResourceManager().getUniformManager();
		const UniformRef &uniform = uniformManager.getUniform(handle);
		const UniformDataBufferPtr &uniformDataBuffer = m_CurrentFrame.UniformDataBuffer;
		assert(count > 0 && count <= uniform.Count && "::ERROR: Invalid uniform count");

		// Writes uniform data to buffer
		uniformDataBuffer->write(&handle, sizeof(UniformHandle));
		uniformDataBuffer->write(&uniform.Type, sizeof(UniformType));
		uniformDataBuffer->write(&count, sizeof(uint8_t));
		uniformDataBuffer->write(data, spr::getUniformSizeByType(uniform.Type) * count);
	}

	void FrameDataManager::setTexture(TextureUnitType unit, const TextureHandle textureHandle, const struct SamplerInfo &samplerInfo) {
		TextureBinding binding;
		binding.Texture = textureHandle;
		binding.Sampler = samplerInfo;

		m_CurrentDrawCall.TextureBindings.resize(unit + 1);
		m_CurrentDrawCall.TextureBindings[unit] = binding;
	}

	void FrameDataManager::setRenderTargetFramebuffer(const uint8_t renderTargetIndex, const FramebufferHandle handle) {
		CHECK_RENDER_TARGET_INDEX(renderTargetIndex);
		m_CurrentFrame.RenderTargets[renderTargetIndex].Framebuffer = handle;
	}

	void FrameDataManager::setFixedFunctionState(const FixedFunctionState &state) {
		m_CurrentDrawCall.FixedFunctionState = state;
	}

	void FrameDataManager::blit(const BlitParameters& blitParams) {
		m_CurrentFrame.BlitRequests.push_back(blitParams);
	}

	void FrameDataManager::setRenderTargetRect(const uint8_t renderTargetIndex, const Rect &rect) {
		CHECK_RENDER_TARGET_INDEX(renderTargetIndex);
		m_CurrentFrame.RenderTargets[renderTargetIndex].ViewportRect = rect;
	}

	void FrameDataManager::setRenderTargetClear(const uint8_t renderTargetIndex, const uint8_t clearFlags) {
		CHECK_RENDER_TARGET_INDEX(renderTargetIndex);
		m_CurrentFrame.RenderTargets[renderTargetIndex].ClearFlags = clearFlags;
	}

}