#pragma once

#include "Handles.h"
#include "FixedFunctionState.h"
#include "MathTypes.h"
#include "ResourceManager/SamplerInfo.h"
#include "ResourceManager/UniformManager.h"
#include "ResourceManager/FramebufferAttachmentType.h"

namespace spr {

	class Context;

	struct TextureBinding {
		TextureHandle Texture;
		SamplerInfo Sampler;
	};

	using TextureUnitType = uint32_t;
	using TextureBindings = std::unordered_map<TextureUnitType, TextureBinding>;

	struct RenderTarget {
		FramebufferHandle Framebuffer = kInvalidHandle;
		Rect ViewportRect;
		uint8_t ClearFlags = spr::AsFlag(FramebufferAttachmentFlags::All);

		static inline const uint8_t kMaxRenderTargets = 32;
	};

	struct DrawCallData {
		FixedFunctionState FixedFunctionState;
		ProgramHandle Program;
		VertexBufferHandle VertexBuffer;
		IndexBufferHandle IndexBuffer;
		TextureBindings TextureBindings;

		uint32_t UniformsStart;
		uint32_t UniformsEnd;

		static_assert(RenderTarget::kMaxRenderTargets < 255 &&
			"spr::ERROR: This code assumes that the maximum amount of render targets is smaller than the uint8 limit.");

		uint8_t RenderTargetIndex = RenderTarget::kMaxRenderTargets;

		inline bool operator>(const DrawCallData& other) const {
			return FixedFunctionState.GetSortKey() > other.FixedFunctionState.GetSortKey();
		}

		inline bool operator<(const DrawCallData& other) const {
			return FixedFunctionState.GetSortKey() < other.FixedFunctionState.GetSortKey();
		}

		inline bool operator==(const DrawCallData& other) const {
			return FixedFunctionState.GetSortKey() == other.FixedFunctionState.GetSortKey();
		}



		void clear();
	};

	struct BlitParameters {
		FramebufferHandle Source, Destination;
		Rect SourceRect, DestinationRect;

		/* Determines which attachments will get blitted. */
		uint32_t BlitMask = 0;
	};

	struct FrameData {
		std::vector<DrawCallData>		DrawCalls;
		std::vector<BlitParameters>		BlitRequests;
		UniformDataBufferPtr			UniformDataBuffer;
		RenderTarget					RenderTargets[RenderTarget::kMaxRenderTargets + 1];

		void clear();
		void sortDrawCalls();
		uint32_t lastDrawCallUniformEnd();
	};

}

namespace spr {

	class FrameDataManager {
	public:
		void init(Context *owner, const struct ContextInfo& info);
		/* Saves current draw call to current frame */
		void submit(uint8_t renderTargetIndex, ProgramHandle &program);
		/* Cleans current frame data. This should be called after rendering a frame */
		void reset();
		/* Sorts draw calls by their sort key */
		void sortDrawCalls();

		void setVertexBuffer(const VertexBufferHandle handle);
		void setIndexBuffer(const IndexBufferHandle handle);
		void setUniform(const UniformHandle handle, const void *data, uint8_t count);
		void setTexture(TextureUnitType unit, const TextureHandle textureHandle, const struct SamplerInfo &samplerInfo);

		void setFixedFunctionState(const FixedFunctionState &state);

		void setRenderTargetFramebuffer(const uint8_t renderTargetIndex, const FramebufferHandle handle);
		void setRenderTargetRect(const uint8_t renderTargetIndex, const Rect &rect);
		void setRenderTargetClear(const uint8_t renderTargetIndex, const uint8_t clearFlags);

		/* Sends a blit request to the renderer. Once spr::render is called and all Draw Calls are resolved, then all blits are executed. */
		void blit(const BlitParameters& blitParams);

	public:
		inline const FrameData &getCurrentFrameData() { return m_CurrentFrame; }

	private:
		FrameData m_CurrentFrame;
		DrawCallData m_CurrentDrawCall;

	private:
		Context *m_Owner;
	};

}