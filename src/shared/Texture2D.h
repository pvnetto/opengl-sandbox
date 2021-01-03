#pragma once

#include <string>

class Texture2D {
public:
	static void LoadTexture(const int textureUnit, const std::string &path);
};