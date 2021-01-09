#pragma once

#include <string>
#include <vector>

struct Texture {
	unsigned int ID;
	std::string Path;

	Texture() {}
	Texture(unsigned int id, std::string path)
	    : ID(id),
	      Path(path) { }
};

class TextureLoader {
public:
	static Texture LoadTexture(const std::string &path, const int textureUnit=0);

private:
	static std::vector<Texture> s_loadedTextures;
};