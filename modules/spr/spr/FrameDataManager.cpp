#include "FrameDataManager.h"

#include <spr/Context.h>
#include <spr/ResourceManager/SamplerInfo.h>

#include <cassert>

namespace spr {

	void FrameData::clear() {
		UniformDataBuffer->reset();
		DrawCalls.clear();
	}

	uint32_t FrameData::lastDrawCallUniformEnd() {
		if (DrawCalls.empty()) return 0;
		return DrawCalls[DrawCalls.size() - 1].UniformsEnd;
	}

	void DrawCallData::clear() {
		Program = kInvalidHandle;
		VertexBuffer = kInvalidHandle;
		IndexBuffer = kInvalidHandle;
		UniformsStart = 0;
		UniformsEnd = 0;
		TextureBindings.empty();
	}

	void FrameDataManager::init(Context* owner) {
		m_Owner = owner;
		m_CurrentFrame.UniformDataBuffer = UniformDataBuffer::alloc();
	}

	void FrameDataManager::submit(ProgramHandle &program) {
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

	void FrameDataManager::setVertexBuffer(const VertexBufferHandle &handle) {
		assert(!m_CurrentDrawCall.VertexBuffer.isValid() && "::ERROR: Vertex buffer was already set for this frame.");
		m_CurrentDrawCall.VertexBuffer = handle;
	}

	void FrameDataManager::setIndexBuffer(const IndexBufferHandle &handle) {
		assert(!m_CurrentDrawCall.IndexBuffer.isValid() && "::ERROR: Index buffer was already set in this draw call");
		m_CurrentDrawCall.IndexBuffer = handle;
	}

	void FrameDataManager::setUniform(const UniformHandle &uniformHandle, const void *data, uint8_t count) {
		assert(uniformHandle.isValid() && "::ERROR: Invalid uniform handle!");
		const UniformManager& uniformManager = m_Owner->getResourceManager().getUniformManager();
		const UniformRef &uniform = uniformManager.getUniform(uniformHandle);
		const UniformDataBufferPtr &uniformDataBuffer = m_CurrentFrame.UniformDataBuffer;
		assert(count > 0 && count <= uniform.Count && "::ERROR: Invalid uniform count");

		// Writes uniform data to buffer
		uniformDataBuffer->write(&uniformHandle, sizeof(UniformHandle));
		uniformDataBuffer->write(&uniform.Type, sizeof(UniformType));
		uniformDataBuffer->write(&count, sizeof(uint8_t));
		uniformDataBuffer->write(data, spr::getUniformSizeByType(uniform.Type) * count);
	}

	void FrameDataManager::setTexture(TextureUnitType unit, const TextureHandle &textureHandle, const struct SamplerInfo &samplerInfo) {
		TextureBinding binding;
		binding.Texture = textureHandle;
		binding.Sampler = samplerInfo;
		m_CurrentDrawCall.TextureBindings.emplace(unit, binding);
	}

}