#pragma once

#include "VertexBufferManagerGL.h"
#include "IndexBufferManagerGL.h"
#include "UniformManagerGL.h"
#include "ShaderManagerGL.h"
#include "ProgramManagerGL.h"

namespace spr {

	class ResourceManagerGL {
	public:
		void init(Context *owner);

		void createVertexBuffer(const VertexBufferHandle &handle, const void *data, uint32_t size, const VertexAttributeLayout &layout);
		void destroy(VertexBufferHandle &handle);

		void createIndexBuffer(const IndexBufferHandle &handle, const void *data, uint32_t size);
		void destroy(IndexBufferHandle &handle);

		void createUniform(const UniformHandle &handle, const UniformRef &uniformRef);
		void destroy(UniformHandle &uniformHandle);

		void createShader(const ShaderHandle& handle, unsigned int shaderType, const char *shaderSrc);
		void destroy(ShaderHandle &shaderHandle);

		void createProgram(const ProgramHandle& programHandle, ShaderHandle &vertexHandle, ShaderHandle &fragmentHandle);
		void destroy(ProgramHandle &programHandle);

	public:
		inline const VertexBufferManagerGL &getVertexBufferManager() const { return m_VertexBufferManager; }
		inline const IndexBufferManagerGL &getIndexBufferManager() const { return m_IndexBufferManager; }
		inline const UniformManagerGL &getUniformManager() const { return m_UniformManager; }
		inline UniformManagerGL &getUniformManager() { return m_UniformManager; }
		inline const ShaderManagerGL& getShaderManager() const { return m_ShaderManager; }
		inline const ProgramManagerGL& getProgramManager() const { return m_ProgramManager; }

	private:
		VertexBufferManagerGL m_VertexBufferManager;
		IndexBufferManagerGL m_IndexBufferManager;
		UniformManagerGL m_UniformManager;
		ShaderManagerGL m_ShaderManager;
		ProgramManagerGL m_ProgramManager;

	private:
		Context *m_Owner = nullptr;
	};

}