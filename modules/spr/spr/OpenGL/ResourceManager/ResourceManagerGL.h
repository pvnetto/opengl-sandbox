#pragma once

#include "VertexBufferManagerGL.h"
#include "IndexBufferManagerGL.h"
#include "FramebufferManagerGL.h"
#include "UniformManagerGL.h"
#include "ShaderManagerGL.h"
#include "ProgramManagerGL.h"
#include "TextureManagerGL.h"

namespace spr {

	class ResourceManagerGL {
	public:
		void init(Context *owner);

		void createVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t byteSize, const VertexAttributeLayout &layout);
		void createInstancedVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout);
		void updateVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t byteSize, uint32_t offset);
		void destroy(VertexBufferHandle &handle);

		void createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size);
		void destroy(IndexBufferHandle &handle);

		void createFramebuffer(const FramebufferHandle &handle, const FramebufferAttachmentMap &attachments);
		void destroy(FramebufferHandle &handle);

		void createUniform(const UniformHandle &handle, const UniformRef &uniformRef);
		void destroy(UniformHandle &uniformHandle);

		void createShader(const ShaderHandle& handle, unsigned int shaderType, const char *shaderSrc);
		void destroy(ShaderHandle &shaderHandle);

		void createProgram(const ProgramHandle& programHandle, ShaderHandle &vertexHandle, ShaderHandle &fragmentHandle);
		void destroy(ProgramHandle &programHandle);

		void createTexture(const TextureHandle &handle, const struct TextureInfo &textureInfo, const void *data);
		void destroy(TextureHandle &handle);

	public:
		inline const VertexBufferManagerGL &getVertexBufferManager() const { return m_VertexBufferManager; }
		inline const IndexBufferManagerGL &getIndexBufferManager() const { return m_IndexBufferManager; }
		inline const FramebufferManagerGL &getFramebufferManager() const { return m_FramebufferManager; }
		inline const UniformManagerGL &getUniformManager() const { return m_UniformManager; }
		inline UniformManagerGL &getUniformManager() { return m_UniformManager; }
		inline const ShaderManagerGL& getShaderManager() const { return m_ShaderManager; }
		inline const ProgramManagerGL& getProgramManager() const { return m_ProgramManager; }
		inline const TextureManagerGL& getTextureManager() const { return m_TextureManager; }
		inline TextureManagerGL& getTextureManager() { return m_TextureManager; }

	private:
		VertexBufferManagerGL m_VertexBufferManager;
		IndexBufferManagerGL m_IndexBufferManager;
		FramebufferManagerGL m_FramebufferManager;
		UniformManagerGL m_UniformManager;
		ShaderManagerGL m_ShaderManager;
		ProgramManagerGL m_ProgramManager;
		TextureManagerGL m_TextureManager;

	private:
		Context *m_Owner = nullptr;
	};

}