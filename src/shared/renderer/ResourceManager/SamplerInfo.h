#pragma once

#include "shared/renderer/Utils.h"

#include <cstdint>

namespace spr {

	enum class FilterMethod : uint8_t {
		Point,
		Linear
	};

	enum class WrappingMethod : uint8_t {
		Repeat,
		Mirror,
		Clamp,
		Border,
	};

	using SamplerHashType = std::size_t;
	struct SamplerInfo {
		FilterMethod MagnificationFilter;
		FilterMethod MinificationFilter;
		WrappingMethod WrappingHorizontal;
		WrappingMethod WrappingVertical;

		SamplerHashType GetHash() const {
			SamplerHashType hash = 0;
			hashCombine(hash,
			            murmurHash2A(&MagnificationFilter, sizeof(uint8_t)),
			            murmurHash2A(&MinificationFilter, sizeof(uint8_t)),
			            murmurHash2A(&WrappingHorizontal, sizeof(uint8_t)),
			            murmurHash2A(&WrappingVertical, sizeof(uint8_t)));
			return hash;
		}
	};

}