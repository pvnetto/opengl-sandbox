#include "ModelLoader.h"

#include <iostream>
 
static void ProcessNode(const std::string &path, std::vector<MeshData>& models, aiNode *node, const aiScene *scene);
static MeshData ProcessMesh(const std::string &path, aiMesh *mesh, const aiScene *scene);
static std::vector<std::string> GetMaterialTexturesByType(const std::string &path, aiMaterial *mat, aiTextureType type);

ModelData ModelLoader::Load(const std::string &path) {
	Assimp::Importer importer;
    auto postprocessConfig = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;      // UVs must be flipped as OpenGL flips images in the Y axis
	const aiScene *scene = importer.ReadFile(path, postprocessConfig);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {      // Checks if loaded scene is valid
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return {};
    }

    ModelData model;
    std::string folder = path.substr(0, path.find_last_of('/'));
    ProcessNode(folder, model.Meshes, scene->mRootNode, scene);
	return model;
}

void ProcessNode(const std::string &path, std::vector<MeshData> &meshes, aiNode *node, const aiScene *scene) {
	// Retrieves meshes from scene
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        int meshIndex = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[meshIndex];
        meshes.push_back(ProcessMesh(path, mesh, scene));
    }

    // Recursively processes all child nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(path, meshes, node->mChildren[i], scene);
    }
}

// Converts assimp aiMesh to a MeshData object
MeshData ProcessMesh(const std::string &path, aiMesh *mesh, const aiScene *scene) {
	MeshData modelData;

    // Fetches all Vertices
	modelData.Vertices.reserve(mesh->mNumVertices * 8);
    for(int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D position = mesh->mVertices[i];
        aiVector3D normal = mesh->mNormals[i];
        aiVector3D uv {0, 0, 0};

        if(mesh->mTextureCoords[0]) {
            uv = mesh->mTextureCoords[0][i];
        }

        modelData.Vertices.push_back(position.x);
		modelData.Vertices.push_back(position.y);
		modelData.Vertices.push_back(position.z);
		modelData.Vertices.push_back(uv.x);
		modelData.Vertices.push_back(uv.y);
		modelData.Vertices.push_back(normal.x);
		modelData.Vertices.push_back(normal.y);
		modelData.Vertices.push_back(normal.z);
    }

    // Fetches all Indices
	modelData.Indices.reserve(mesh->mNumFaces * 3);
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const auto& face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
			modelData.Indices.push_back(face.mIndices[j]);
        }
    }

    // Fetches textures referenced by Materials
    if(mesh->mMaterialIndex >= 0) {
        const auto& material = scene->mMaterials[mesh->mMaterialIndex];
		const auto& diffuseTextures = GetMaterialTexturesByType(path, material, aiTextureType_DIFFUSE);
		const auto& specularTextures = GetMaterialTexturesByType(path, material, aiTextureType_SPECULAR);

        auto &textureFilenames = modelData.TextureFilenames;
		textureFilenames.insert(textureFilenames.end(), diffuseTextures.begin(), diffuseTextures.end());
		textureFilenames.insert(textureFilenames.end(), specularTextures.begin(), specularTextures.end());
    }

    return modelData;
}

std::vector<std::string> GetMaterialTexturesByType(const std::string &path, aiMaterial *mat, aiTextureType type) {
	std::vector<std::string> textureFilenames;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString textureFilename;
		mat->GetTexture(type, i, &textureFilename);
		textureFilenames.push_back(path + "/" + textureFilename.C_Str());
    }

    return textureFilenames;
}
