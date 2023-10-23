#pragma once

#include "Handles.h"
#include "FixedFunctionState.h"
#include "MathTypes.h"
#include "ResourceManager/FramebufferAttachmentType.h"
#include "ResourceManager/SamplerInfo.h"
#include "ResourceManager/UniformManager.h"

namespace spr {

	class Context;

	struct TextureBinding {
		TextureHandle Texture;
		SamplerInfo Sampler;

		bool operator==(const TextureBinding& other) const {
			return Texture == other.Texture && Sampler == other.Sampler;
		}
	};

	using TextureUnitType = uint8_t;
	using TextureBindings = std::vector<TextureBinding>;

	struct RenderTarget {
		FramebufferHandle Framebuffer = k_InvalidHandle;
		Rect ViewportRect;
		uint8_t ClearFlags = spr::AsFlag(FramebufferAttachmentFlags::All);

		static inline const uint8_t kMaxRenderTargets = 32;
	};

	// Refer to this for draw call sorting: https://realtimecollisiondetection.net/blog/?p=86
	struct DrawCallSortKey {
		// 1 byte - blending
		uint8_t bIsBlendingDisabled		: 1;
		uint8_t BlendingEquation		: 3;
		//uint8_t						: 4;

		// 2 bytes - program
		HandleType ShaderProgram;

		// 8 bytes - texture bindings
		std::size_t TextureStateHash;

		inline bool operator>(const DrawCallSortKey &otherKey) const {
			// -0: 1st different byte in other is greater than this
			//  0: Equal
			// +0: 1st different byte in this is greater than other
			return memcmp(this, &otherKey, sizeof(DrawCallSortKey)) > 0;
		}

		inline bool operator<(const DrawCallSortKey &otherKey) const {
			return memcmp(this, &otherKey, sizeof(DrawCallSortKey)) < 0;
		}

		inline bool operator==(const DrawCallSortKey &otherKey) const {
			return memcmp(this, &otherKey, sizeof(DrawCallSortKey)) == 0;
		}
	};

	struct DrawCallData {
		FixedFunctionState FixedFunctionState;

		std::vector<VertexBufferHandle> VertexBuffers;
		IndexBufferHandle IndexBuffer;
		ProgramHandle Program;

		TextureBindings TextureBindings;

		uint32_t UniformsStart;
		uint32_t UniformsEnd;

		static_assert(RenderTarget::kMaxRenderTargets < 255 &&
			"spr::ERROR: This code assumes that the maximum amount of render targets is smaller than the uint8 limit.");

		uint8_t RenderTargetIndex = RenderTarget::kMaxRenderTargets;

		inline bool operator>(const DrawCallData& other) const {
			return getSortKey() > other.getSortKey();
		}

		inline bool operator<(const DrawCallData& other) const {
			return getSortKey() < other.getSortKey();
		}

		inline bool operator==(const DrawCallData& other) const {
			return getSortKey() == other.getSortKey();
		}

		void clear();
		DrawCallSortKey getSortKey() const;

	};

	struct BlitParameters {
		FramebufferHandle Source, Destination;
		Rect SourceRect, DestinationRect;

		/* Determines which attachments will get blitted. */
		uint32_t BlitMask = 0;
	};

	enum class DrawCallSortMode {
		Default,
		OrderDependent,
	};

	struct FrameData {
		std::vector<DrawCallData>		DrawCalls;
		std::vector<BlitParameters>		BlitRequests;
		UniformDataBufferPtr			UniformDataBuffer;
		RenderTarget					RenderTargets[RenderTarget::kMaxRenderTargets + 1];
		DrawCallSortMode				SortMode = DrawCallSortMode::Default;

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
		/* Sets sort mode for draw calls */
		void setDrawCallSortMode(DrawCallSortMode sortMode);

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