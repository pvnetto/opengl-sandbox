#include "SimpleRenderer.h"

namespace spr {

	static Context s_Context;

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

	void setUniform(const UniformHandle& uniformHandle, const void* data, uint8_t count /*= 1*/) {
		s_Context.getFrameDataManager().setUniform(uniformHandle, data, count);
	}

	void setTexture(TextureUnitType unit, const TextureHandle& textureHandle, const struct SamplerInfo& samplerInfo) {
		s_Context.getFrameDataManager().setTexture(unit, textureHandle, samplerInfo);
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

	UniformHandle createUniform(const char *name, UniformType type, uint32_t count /*= 1*/) {
		return s_Context.getResourceManager().createUniform(name, type, count);
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

	TextureHandle createTexture(const struct TextureInfo& textureInfo, const void* data) {
		return s_Context.getResourceManager().createTexture(textureInfo, data);
	}

	void destroy(TextureHandle& handle) {
		s_Context.getResourceManager().destroy(handle);
	}

}

