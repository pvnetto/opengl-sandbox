#pragma once

// TODO: Remove all of these one by one by refactoring them into components that make sense
#include "Context.h"
#include "Handles.h"
#include "Runtime.h"
#include "Inputs.h"
#include "ResourceManager/VertexAttributeLayout.h"

#include "ResourceManager/ShaderType.h"
#include "ResourceManager/TextureInfo.h"

namespace spr {
	struct ProgramHandle;
	struct IndexBufferHandle;
}

namespace spr {

	void init();
	void shutdown();

	void submit(ProgramHandle &program);
	void clean();
	
	void clear();
	void render();
	void flush();

	void setVertexBuffer(const VertexBufferHandle &handle);
	void setIndexBuffer(const IndexBufferHandle &handle);
	void setUniform(const UniformHandle &uniformHandle, const void *data, uint8_t count = 1);
	void setTexture(TextureUnitType unit, const TextureHandle &textureHandle, const struct SamplerInfo &samplerInfo = SamplerInfo::Default);

    VertexBufferHandle createVertexBuffer(const void *data, uint32_t size, const VertexAttributeLayout &layout);
	void destroy(VertexBufferHandle &handle);

    IndexBufferHandle createIndexBuffer(const void *data, uint32_t size);
	void destroy(IndexBufferHandle &handle);

	UniformHandle createUniform(const char *name, UniformType type, uint32_t count = 1);
	void destroy(UniformHandle &uniformHandle);

	ShaderHandle createShader(unsigned int shaderType, const char *shaderSrc);
	void destroy(ShaderHandle &handle);

	ProgramHandle createProgram(ShaderHandle &vertexHandle, ShaderHandle &fragmentHandle, bool destroyShaders = true);
	void destroy(ProgramHandle &handle);

	TextureHandle createTexture(const struct TextureInfo &textureInfo, const void *data);
	void destroy(TextureHandle &handle);

}
