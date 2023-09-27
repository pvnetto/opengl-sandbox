#pragma once

#include <cstdint>

namespace spr {

	enum class TextureFormat {
		None,
		RGB8,
		RGBA8,
	};

	struct TextureInfo {
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Depth = 1;
		uint32_t MipCount = 1;
		TextureFormat Format;
	};

}