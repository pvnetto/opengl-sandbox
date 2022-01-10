#include "Texture2D.h"

#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

std::vector<Texture> TextureLoader::s_loadedTextures = std::vector<Texture>();

Texture TextureLoader::LoadTexture(const std::string& path, const int index) {
	for(int i = 0; i < s_loadedTextures.size(); i++) {
		if(std::strcmp(s_loadedTextures[i].Path.data(), path.c_str()) == 0) {
			return s_loadedTextures[i];
		}
	}

	int texWidth, texHeight, numOfChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load(path.c_str(), &texWidth, &texHeight, &numOfChannels, 0);

	if (!texData) {
		std::cout << "Couldn't load texture\n";
		return Texture{ 0, "" };
	}

	auto colorMode = GL_RGB;
	if(numOfChannels == 1)
		colorMode = GL_R;
	else if(numOfChannels == 3)
		colorMode = GL_RGB;
	else if(numOfChannels == 4)
		colorMode = GL_RGBA;

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

	Texture texture { tex, path };
	s_loadedTextures.push_back(texture);

	return texture;
}