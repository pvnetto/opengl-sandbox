#pragma once

#include "shared/renderer/Handles.h"
#include "shared/renderer/FrameDataManager.h"

#include <cstdint>
#include <unordered_map>

namespace spr {

	struct TextureInstanceGL {
		uint32_t ID;

		void create(const struct TextureInfo &textureInfo, const void *data);
		void destroy();
		void bind(TextureUnitType unit) const;
	};

	// TODO: Delete all samplers on shutdown
	struct SamplerInstanceGL {
		uint32_t ID;

		void create(const SamplerInfo &samplerInfo);
		void destroy();
		void bind(TextureUnitType unit) const;
	};

	using SamplerInstanceMap = std::unordered_map<SamplerHashType, SamplerInstanceGL>;

	class TextureManagerGL {
	public:
		void createTexture(const TextureHandle &handle, const struct TextureInfo &textureInfo, const void *data);
		void destroy(TextureHandle &handle);

		const TextureInstanceGL &getTexture(const TextureHandle &handle) const;

		const SamplerInstanceGL &findOrCreateSampler(const SamplerInfo &samplerInfo);

	private:
		TextureInstanceGL m_Textures[TextureHandle::capacity];
		SamplerInstanceMap m_Samplers;
	};

}