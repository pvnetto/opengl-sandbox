#include "SimpleRenderer.h"

namespace spr {

	static Context s_Context;

	void init(const ContextInfo &info) {
		s_Context.init(info);
	}

	void shutdown() {
		s_Context.shutdown();
	}

	void submit(ProgramHandle &program) {
		const uint8_t defaultRenderTargetIndex = 0;
		s_Context.getFrameDataManager().submit(defaultRenderTargetIndex, program);
	}

	void submit(uint8_t renderTargetIndex, ProgramHandle& program) {
		s_Context.getFrameDataManager().submit(renderTargetIndex, program);
	}

	void clean() {
		s_Context.getFrameDataManager().reset();
	}

	void render() {
		s_Context.getFrameDataManager().sortDrawCalls();

		const FrameData &frameData = s_Context.getFrameDataManager().getCurrentFrameData();
		s_Context.getRendererContext().render(frameData);
	}

	void flush() {
		render();
		clean();
	}

	void setDrawCallSortMode(const DrawCallSortMode sortMode) {
		s_Context.getFrameDataManager().setDrawCallSortMode(sortMode);
	}

	void setVertexBuffer(const VertexBufferHandle &handle) {
		s_Context.getFrameDataManager().setVertexBuffer(handle);
	}

	void setIndexBuffer(const IndexBufferHandle& handle) {
		s_Context.getFrameDataManager().setIndexBuffer(handle);
	}

	void setUniform(const UniformHandle& uniformHandle, const void* data, uint8_t count /*= 1*/) {
		assert(data != NULL);
		s_Context.getFrameDataManager().setUniform(uniformHandle, data, count);
	}

	void setTexture(TextureUnitType unit, const TextureHandle& textureHandle, const struct SamplerInfo& samplerInfo) {
		s_Context.getFrameDataManager().setTexture(unit, textureHandle, samplerInfo);
	}

	void setFixedFunctionState(const FixedFunctionState &state) {
		s_Context.getFrameDataManager().setFixedFunctionState(state);
	}

	void setRenderTargetFramebuffer(const uint8_t renderTargetIndex, const FramebufferHandle handle) {
		s_Context.getFrameDataManager().setRenderTargetFramebuffer(renderTargetIndex, handle);
	}

	void setRenderTargetRect(const uint8_t renderTargetIndex, const Rect &rect) {
		s_Context.getFrameDataManager().setRenderTargetRect(renderTargetIndex, rect);
	}

	void setRenderTargetClear(const uint8_t renderTargetIndex /*= 0*/, const uint8_t clearFlags /*= 0b111*/) {
		s_Context.getFrameDataManager().setRenderTargetClear(renderTargetIndex, clearFlags);
	}

	VertexBufferHandle createVertexBuffer(const void *data, uint32_t byteSize, const VertexAttributeLayout &layout) {
		return s_Context.getResourceManager().createVertexBuffer(data, byteSize, layout);
	}

	VertexBufferHandle createInstancedVertexBuffer(const void *data, uint32_t byteSize, uint32_t instanceCount, const VertexAttributeLayout &layout) {
		return s_Context.getResourceManager().createInstancedVertexBuffer(data, byteSize, instanceCount, layout);
	}

	void updateInstancedVertexBuffer(const VertexBufferHandle handle, const void *data, uint32_t byteSize, uint32_t offset) {
		s_Context.getResourceManager().updateVertexBuffer(handle, data, byteSize, offset);
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

	FramebufferHandle createFramebuffer(const FramebufferAttachmentMap &attachments) {
		return s_Context.getResourceManager().createFramebuffer(attachments);
	}

	void destroy(FramebufferHandle &handle) {
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

	void blit(const BlitParameters &blitParams) {
		s_Context.getFrameDataManager().blit(blitParams);
	}

}

