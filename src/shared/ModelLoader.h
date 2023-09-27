#pragma once

#include <spr/SimpleRenderer.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

struct MeshData {
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<std::string> TextureFilenames;
};

struct ModelData {
	std::vector<MeshData> Meshes;
};

class ModelLoader {
public:
    static ModelData Load(const std::string& path);
};