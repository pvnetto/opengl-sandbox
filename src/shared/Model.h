#pragma once

#include "Mesh.h"
#include "Texture2D.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class Model {
public:
    Model() = default;
    Model(const char* path);

    void Draw(Shader& shader);

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
};