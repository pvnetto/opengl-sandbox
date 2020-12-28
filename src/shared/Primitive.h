#pragma once

#include <vector>

class PrimitiveShape {
public:
	static std::vector<float> Cube() {
		return {
		    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		};;
	}
};