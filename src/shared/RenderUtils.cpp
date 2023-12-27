#include "RenderUtils.h"

#include "Utils.h"
#include "ModelLoader.h"

#include <iostream>
#include <stb_image.h>

namespace Utils {

	static spr::VertexAttributeLayout GetMeshLayout() {
		spr::VertexAttributeLayout layout;
		layout.begin()
		    .add({"inPosition", spr::AttributeType::Float, 3})
		    .add({"inUV", spr::AttributeType::Float, 2})
		    .add({"inNormal", spr::AttributeType::Float, 3})
		    .end();
		return layout;
	}

	PrimitiveData GetQuadData() {
		// clang-format off
		static float vertices[] = {
			// Position					// UVs				// Normals (probably incorrect!)
			-0.5f, -0.5f, 0.0f,         0.0f, 0.0f,			0.f, 0.f, 1.f,
			0.5f, -0.5f, 0.0f,          1.0f, 0.0f,			0.f, 0.f, 1.f,
			0.5f, 0.5f, 0.0f,           1.0f, 1.0f,			0.f, 0.f, 1.f,
			-0.5f, 0.5f, 0.0f,          0.0f, 1.0f,			0.f, 0.f, 1.f,
		};

		static unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};
		// clang-format on

		static PrimitiveData quad;
		quad.Layout = GetMeshLayout();

		quad.Vertices = vertices;
		quad.VerticesSize = sizeof(vertices) / sizeof(float);
		quad.NumVertices = quad.VerticesSize / (quad.Layout.getStride() / sizeof(float));
		
		quad.Indices = indices;
		quad.NumIndices = sizeof(indices) / sizeof(unsigned int);
		return quad;
	}

	PrimitiveData GetScreenQuadData() {
		// clang-format off
		static float vertices[] = {
			// Position					// UVs				// Normals (probably incorrect!)
			-1.f, -1.f, 0.0f,         0.0f, 0.0f,			0.f, 0.f, 1.f,
			1.f, -1.f, 0.0f,          1.0f, 0.0f,			0.f, 0.f, 1.f,
			1.f, 1.f, 0.0f,           1.0f, 1.0f,			0.f, 0.f, 1.f,
			-1.f, 1.f, 0.0f,          0.0f, 1.0f,			0.f, 0.f, 1.f,
		};

		static unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};
		// clang-format on

		static PrimitiveData quad;
		quad.Layout = GetMeshLayout();

		quad.Vertices = vertices;
		quad.VerticesSize = sizeof(vertices) / sizeof(float);
		quad.NumVertices = quad.VerticesSize / (quad.Layout.getStride() / sizeof(float));

		quad.Indices = indices;
		quad.NumIndices = sizeof(indices) / sizeof(unsigned int);
		return quad;
	}

	PrimitiveData GetCubeData() {
		// clang-format off
		static float vertices[] = {
		    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		    
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		    
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		    
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		    
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
		};
		// clang-format on

		static PrimitiveData cube;
		cube.Layout = GetMeshLayout();

		cube.Vertices = vertices;
		cube.VerticesSize = sizeof(vertices) / sizeof(float);
		cube.NumVertices = cube.VerticesSize / (cube.Layout.getStride() / sizeof(float));
		
		return cube;
	}

	spr::VertexBufferHandle LoadCube() {
		Utils::PrimitiveData cube = Utils::GetCubeData();
		return spr::createVertexBuffer(cube.Vertices, cube.VerticesSize * sizeof(float), cube.Layout);
	}

	ModelInstance LoadModel(const char* path) {
		ModelData model = ModelLoader::Load(path);

		std::unordered_map<std::string, spr::TextureHandle> loadedTextures;

		ModelInstance modelInstance;
		modelInstance.Meshes.reserve(model.Meshes.size());
		for (const MeshData &mesh : model.Meshes) {
			MeshInstance meshInstance;
			meshInstance.VertexBuffer = spr::createVertexBuffer(mesh.Vertices.data(), mesh.Vertices.size() * sizeof(float), GetMeshLayout());
			if (mesh.Indices.size() > 0) {
				meshInstance.IndexBuffer = spr::createIndexBuffer(mesh.Indices.data(), mesh.Indices.size() * sizeof(unsigned int));
			}

			for (const std::string &texturePath : mesh.TextureFilenames) {
				if (loadedTextures.count(texturePath) == 0) {
					loadedTextures[texturePath] = LoadTexture(texturePath.c_str());
				}
				meshInstance.Textures.push_back(loadedTextures[texturePath]);
			}
			modelInstance.Meshes.push_back(meshInstance);
		}

		return modelInstance;
	}

	spr::TextureHandle LoadTexture(const char *path) {
		int texWidth, texHeight, numOfChannels;
		unsigned char *textureData = stbi_load(path, &texWidth, &texHeight, &numOfChannels, 0);
		if (!textureData) {
			assert(false && "::ERROR: Couldn't load texture");
		}

		spr::TextureInfo textureInfo;
		textureInfo.Width = texWidth;
		textureInfo.Height = texHeight;
		textureInfo.Format = numOfChannels == 4 ? spr::TextureFormat::RGBA8 : spr::TextureFormat::RGB8;
		spr::TextureHandle handle = spr::createTexture(textureInfo, textureData);
		stbi_image_free(textureData);
		return handle;
	}

	spr::ProgramHandle LoadShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
		std::string vertexSrc = Utils::ReadShaderFile(vertexShaderPath);
		spr::ShaderHandle vertexShader = spr::createShader(SPR_VERTEX_SHADER, vertexSrc.c_str());

		std::string fragSrc = Utils::ReadShaderFile(fragmentShaderPath);
		spr::ShaderHandle fragmentShader = spr::createShader(SPR_FRAGMENT_SHADER, fragSrc.c_str());

		return spr::createProgram(vertexShader, fragmentShader);
	}

	void UnloadModel(ModelInstance& model) {
		for (auto &mesh : model.Meshes) {
			spr::destroy(mesh.VertexBuffer);
			spr::destroy(mesh.IndexBuffer);
			for (auto &texture : mesh.Textures) {
				spr::destroy(texture);
			}
		}
	}

}