#pragma once

#include "renderer/SimpleRenderer.h"

namespace Utils {

	struct PrimitiveData {
		float *Vertices = nullptr;
		unsigned int *Indices = nullptr;

		// Number of vertices. Each vertex has position, normal and UV.
		std::size_t NumVertices = 0;

		// Number of indices. Each index is an uint.
		std::size_t NumIndices = 0;

		// Number of elements in the Vertices array
		std::size_t VerticesSize = 0;
		
		spr::VertexAttributeLayout Layout;
	};

	struct MeshInstance {
		spr::VertexBufferHandle VertexBuffer;
		spr::IndexBufferHandle IndexBuffer;
		std::vector<spr::TextureHandle> Textures;
	};

	struct ModelInstance {
		std::vector<MeshInstance> Meshes;
	};

	PrimitiveData GetQuadData();
	PrimitiveData GetCubeData();

	spr::VertexBufferHandle LoadCube();
	ModelInstance LoadModel(const char *path);
	spr::TextureHandle LoadTexture(const char *path);
	spr::ProgramHandle LoadShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);

	void UnloadModel(ModelInstance &model);

}