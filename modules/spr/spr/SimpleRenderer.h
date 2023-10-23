#pragma once

// TODO: Remove all of these one by one by refactoring them into modules/classes
#include "Context.h"
#include "Handles.h"
#include "ResourceManager/VertexAttributeLayout.h"

#include "ResourceManager/ShaderType.h"
#include "ResourceManager/TextureInfo.h"
#include "ResourceManager/FramebufferAttachmentType.h"

namespace spr {
	struct ProgramHandle;
	struct IndexBufferHandle;
}

namespace spr {

	void init(const ContextInfo& info);
	void shutdown();

	void submit(ProgramHandle &program);
	void submit(uint8_t renderTargetIndex, ProgramHandle &program);
	void clean();
	
	void render();
	void flush();

	void setDrawCallSortMode(const DrawCallSortMode sortMode);
	void setVertexBuffer(const VertexBufferHandle &handle);
	void setIndexBuffer(const IndexBufferHandle &handle);
	void setUniform(const UniformHandle &uniformHandle, const void *data, uint8_t count = 1);
	void setTexture(TextureUnitType unit, const TextureHandle &textureHandle, const struct SamplerInfo &samplerInfo = SamplerInfo::Default);

	void setFixedFunctionState(const FixedFunctionState &state);

	void setRenderTargetFramebuffer(const uint8_t renderTargetIndex, const FramebufferHandle handle);
	void setRenderTargetRect(const uint8_t renderTargetIndex, const Rect &rect);
	void setRenderTargetClear(const uint8_t renderTargetIndex = 0, const uint8_t clearFlags = spr::AsFlag(FramebufferAttachmentFlags::All));

    VertexBufferHandle createVertexBuffer(const void *data, uint32_t byteSize, const VertexAttributeLayout &layout);
    VertexBufferHandle createInstancedVertexBuffer(const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout);
	void updateInstancedVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t offset);
	void destroy(VertexBufferHandle &handle);

    IndexBufferHandle createIndexBuffer(const void *data, uint32_t size);
	void destroy(IndexBufferHandle &handle);

	FramebufferHandle createFramebuffer(const FramebufferAttachmentMap &attachments);
	void destroy(FramebufferHandle &handle);

	UniformHandle createUniform(const char *name, UniformType type, uint32_t count = 1);
	void destroy(UniformHandle &uniformHandle);

	ShaderHandle createShader(unsigned int shaderType, const char *shaderSrc);
	void destroy(ShaderHandle &handle);

	ProgramHandle createProgram(ShaderHandle &vertexHandle, ShaderHandle &fragmentHandle, bool destroyShaders = true);
	void destroy(ProgramHandle &handle);

	TextureHandle createTexture(const struct TextureInfo &textureInfo, const void *data);
	void destroy(TextureHandle &handle);

	void blit(const BlitParameters &blitParams);

}
