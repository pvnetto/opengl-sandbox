#pragma once

#include <cstdint>

struct Rect {
	uint32_t OffsetX = 0, OffsetY = 0, Width = 0, Height = 0;

	Rect() = default;
	Rect(uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height) : OffsetX(offsetX), OffsetY(offsetY), Width(width), Height(height) {}

	inline bool operator==(const Rect& other) const {
		return OffsetX == other.OffsetX && OffsetY == other.OffsetY && Width == other.Width && Height == other.Height;
	}

	inline bool operator!=(const Rect& other) const {
		return !(*this == other);
	}
};
