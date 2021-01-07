#include "Texture2D.h"

#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

int Texture2D::LoadTexture(const int index, const std::string& path) {
	// 4) Loads textures
	int texWidth, texHeight, numOfChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load(path.c_str(), &texWidth, &texHeight, &numOfChannels, 0);


	if (!texData) {
		std::cout << "Couldn't load texture\n";
		return 0;
	}

	auto colorMode = path.find(".png") == std::string::npos ? GL_RGB : GL_RGBA; 

	unsigned int tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, colorMode, texWidth, texHeight, 0, colorMode, GL_UNSIGNED_BYTE, texData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(tex);

	stbi_image_free(texData);

	return tex;
}