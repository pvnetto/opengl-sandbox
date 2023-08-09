#include "SimpleRenderer.h"

// TODO: Once everything is refactored, remove this getter!!
namespace spr {

	static Context s_Context;

	Context* getContext() {
		return &s_Context;
	}

}

namespace spr {

	void init() {
		s_Context.init();
	}

	void shutdown() {
		s_Context.shutdown();
	}

	void submit(ProgramHandle &program) {
		s_Context.getFrameDataManager().submit(program);
	}

	void clean() {
		s_Context.getFrameDataManager().reset();
	}

	void clear() {
		s_Context.getRendererContext().clear();
	}

	void render() {
		const FrameData &frameData = s_Context.getFrameDataManager().getCurrentFrameData();
		s_Context.getRendererContext().render(frameData);
	}

	void setVertexBuffer(const VertexBufferHandle& handle) {
		s_Context.getFrameDataManager().setVertexBuffer(handle);
	}

	void setIndexBuffer(const IndexBufferHandle& handle) {
		s_Context.getFrameDataManager().setIndexBuffer(handle);
	}

	void setUniform(const UniformHandle& uniformHandle, const void* data) {
		s_Context.getFrameDataManager().setUniform(uniformHandle, data);
	}

	VertexBufferHandle createVertexBuffer(const void* data, uint32_t size, const VertexAttributeLayout& layout) {
		return s_Context.getResourceManager().createVertexBuffer(data, size, layout);
	}
	
	void destroy(VertexBufferHandle& handle) {
		s_Context.getResourceManager().destroy(handle);
	}

	IndexBufferHandle createIndexBuffer(const void* data, uint32_t size) {
		return s_Context.getResourceManager().createIndexBuffer(data, size);
	}

	void destroy(IndexBufferHandle& handle) {
		s_Context.getResourceManager().destroy(handle);
	}

	UniformHandle createUniform(const char* name, UniformType type) {
		return s_Context.getResourceManager().createUniform(name, type);
	}

	void destroy(UniformHandle& uniformHandle) {
		s_Context.getResourceManager().destroy(uniformHandle);
	}

	ShaderHandle createShader(unsigned int shaderType, const char* shaderSrc) {
		return s_Context.getResourceManager().createShader(shaderType, shaderSrc);
	}

	void destroy(ShaderHandle& handle) {
		s_Context.getResourceManager().destroy(handle);
	}

	ProgramHandle createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders) {
		return s_Context.getResourceManager().createProgram(vertexHandle, fragmentHandle, destroyShaders);
	}

	void destroy(ProgramHandle& handle) {
		s_Context.getResourceManager().destroy(handle);
	}

}


