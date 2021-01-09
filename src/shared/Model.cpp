#include "Model.h"

Model::Model(const char *path) {
	LoadModel(path);
}

void Model::Draw(Shader &shader) {
	for (unsigned int i = 0; i < m_meshes.size(); i++)
		m_meshes[i].Draw(shader);
}

void Model::LoadModel(std::string path) {
	Assimp::Importer importer;
    auto postprocessConfig = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;      // UVs must be flipped as OpenGL flips images in the Y axis
	const aiScene *scene = importer.ReadFile(path, postprocessConfig);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {      // Checks if loaded scene is valid
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

/// Processes all meshes from all child nodes recursively
void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        int meshIndex = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[meshIndex];       // Retrieves mesh from scene
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);         // Processes all child nodes
    }
}

/// Converts assimp mesh data to a Mesh object
Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // processes vertex data
    for(int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D coordinates = mesh->mVertices[i];
        aiVector3D normal = mesh->mNormals[i];
        aiVector3D uv {0, 0, 0};

        if(mesh->mTextureCoords[0]) {
            uv = mesh->mTextureCoords[0][i];
        }

        Vertex vertex { coordinates.x, coordinates.y, coordinates.z, uv.x, uv.y, normal.x, normal.y, normal.z };
        vertices.push_back(vertex);
    }

    // processes indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // processes texture data from materials
    if(mesh->mMaterialIndex >= 0) {
        auto material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
        std::vector<Texture> specularTextures = LoadMaterialTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
    }

    return Mesh{ vertices, indices, textures };
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString path;
        mat->GetTexture(type, i, &path);

        textures.push_back(TextureLoader::LoadTexture(m_directory + "/" + path.C_Str()));
    }

    return textures;
}