#pragma once

#include "Handles.h"
#include "ResourceManager/SamplerInfo.h"
#include "ResourceManager/UniformManager.h"

namespace spr {

	class Context;

	struct TextureBinding {
		TextureHandle Texture;
		SamplerInfo Sampler;
	};

	using TextureUnitType = uint32_t;
	using TextureBindings = std::unordered_map<TextureUnitType, TextureBinding>;
	
	struct DrawCallData {
		ProgramHandle Program;
		VertexBufferHandle VertexBuffer;
		IndexBufferHandle IndexBuffer;
		TextureBindings TextureBindings;

		uint32_t UniformsStart;
		uint32_t UniformsEnd;

		void clear();
	};

	struct FrameData {
		UniformDataBufferPtr UniformDataBuffer;
		std::vector<DrawCallData> DrawCalls;

		void clear();
		uint32_t lastDrawCallUniformEnd();
	};

}

namespace spr {

	class FrameDataManager {
	public:
		void init(Context *owner);
		/* Saves current draw call to current frame */
		void submit(ProgramHandle &program);
		/* Cleans current frame data. This should be called after rendering a frame */
		void reset();

		void setVertexBuffer(const VertexBufferHandle &handle);
		void setIndexBuffer(const IndexBufferHandle &handle);
		void setUniform(const UniformHandle &uniformHandle, const void *data);
		void setTexture(TextureUnitType unit, const TextureHandle &textureHandle, const struct SamplerInfo &samplerInfo);

	public:
		inline const FrameData &getCurrentFrameData() { return m_CurrentFrame; }

	private:
		FrameData m_CurrentFrame;
		DrawCallData m_CurrentDrawCall;

	private:
		Context *m_Owner;
	};

}