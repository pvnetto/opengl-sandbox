#pragma once

#include "spr/Handles.h"

#include <unordered_map>

namespace spr {

	enum class FramebufferAttachmentType : uint8_t {
		ColorMIN,
		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
		ColorMAX,
		Depth,
		Stencil,
		DepthStencil,
	};

	// clang-format off
	enum class FramebufferAttachmentFlags : uint8_t {
		Color		= 1 << 0,
		Depth		= 1 << 1,
		Stencil		= 1 << 2,
		All			= 0b111,
	};
	// clang-format on

	template <typename T>
	uint8_t AsFlag(T enumValue) {
		return ((uint8_t) enumValue);
	}

	template<typename... Args>
	uint8_t AsMask(Args&&... args) {
		return (0 | ... | (uint8_t) args);
	}

	template <typename T>
	bool HasFlag(const uint8_t flags, T enumValue) {
		return flags & ((uint8_t) enumValue);
	}

	using FramebufferAttachmentMap = std::unordered_map<FramebufferAttachmentType, TextureHandle>;


}