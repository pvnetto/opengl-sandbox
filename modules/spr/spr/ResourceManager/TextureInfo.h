#pragma once

#include <cstdint>

namespace spr {

	enum class TextureFormat {
		None,
		RGB8,
		RGBA8,
		DEPTH_24,
		STENCIL_8,
		DEPTH_24_STENCIL_8,
	};

	enum class TextureFlags : uint32_t {
		IsRenderTargetTexture = 1 << 0,
	};

	struct TextureInfo {
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Depth = 1;
		uint32_t MipCount = 1;
		uint32_t NumSamples = 1;
		TextureFormat Format;
		uint32_t Flags = 0;
	};

}