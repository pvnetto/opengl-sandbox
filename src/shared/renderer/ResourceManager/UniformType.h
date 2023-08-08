#pragma once

#include <cstdint>
#include <cassert>

namespace spr {

	enum class UniformType {
		Integer,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3x3,
		Mat4x4,
		Sampler,
	};

     static uint32_t getUniformSizeByType(UniformType type) {
	     switch(type) {
	     case UniformType::Float:
	         return sizeof(float);
	     case UniformType::Sampler:
	     case UniformType::Integer:
	         return sizeof(int);
	     case UniformType::Vec2:
	         return sizeof(float) * 2;
	     case UniformType::Vec3:
	         return sizeof(float) * 3;
	     case UniformType::Vec4:
	         return sizeof(float) * 4;
	     default:
	         assert(false && "::ERROR: Undefined size for specified uniform type");
	         return 0;
	     }
	 }

}