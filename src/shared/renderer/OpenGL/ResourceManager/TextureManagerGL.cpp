#include "TextureManagerGL.h"

#include "TextureUtilsGL.h"

#include <glad/glad.h>
#include <cassert>

namespace spr {

	void TextureManagerGL::createTexture(const TextureHandle &handle, const struct TextureInfo &textureInfo, const void *data) {
		m_Textures[handle.idx].create(textureInfo, data);
	}

	void TextureManagerGL::destroy(TextureHandle &handle) {
		m_Textures[handle.idx].destroy();
	}

	const TextureInstanceGL &TextureManagerGL::getTexture(const TextureHandle &handle) const {
		assert(handle.isValid() && "::ERROR: Invalid texture");
		return m_Textures[handle.idx];
	}

	const SamplerInstanceGL &TextureManagerGL::findOrCreateSampler(const SamplerInfo &samplerInfo) {
		SamplerHashType samplerHash = samplerInfo.GetHash();
		if (m_Samplers.count(samplerHash) == 0) {
			auto& sampler = m_Samplers.emplace(samplerHash, SamplerInstanceGL{});
			m_Samplers[samplerHash].create(samplerInfo);
		}
		return m_Samplers[samplerHash];
	}

	void TextureInstanceGL::create(const struct TextureInfo &textureInfo, const void *data) {
		glCreateTextures(GL_TEXTURE_2D, 1, &ID);

		if (textureInfo.Depth == 1) {
			glTextureStorage2D(ID, textureInfo.MipCount, getInternalFormatGL(textureInfo.Format), textureInfo.Width, textureInfo.Height);
			glTextureSubImage2D(ID, 0, 0, 0, textureInfo.Width, textureInfo.Height, getFormatGL(textureInfo.Format), GL_UNSIGNED_BYTE, data);		
		}
		else {
			assert(false && "::ERROR: 3D textures are not supported yet");
		}

		if (textureInfo.MipCount > 1) {
			glGenerateTextureMipmap(ID);
		}
	}

	void TextureInstanceGL::destroy() {
		glDeleteTextures(1, &ID);
	}

	void TextureInstanceGL::bind(TextureUnitType unit) const {
		glBindTextureUnit(unit, ID);
	}

	void SamplerInstanceGL::create(const struct SamplerInfo &samplerInfo) {
		glCreateSamplers(1, &ID);

		glSamplerParameteri(ID, GL_TEXTURE_WRAP_S, getWrappingMethodGL(samplerInfo.WrappingHorizontal));
		glSamplerParameteri(ID, GL_TEXTURE_WRAP_T, getWrappingMethodGL(samplerInfo.WrappingVertical));
		glSamplerParameteri(ID, GL_TEXTURE_MAG_FILTER, getFilterMethodGL(samplerInfo.MagnificationFilter));
		glSamplerParameteri(ID, GL_TEXTURE_MIN_FILTER, getFilterMethodGL(samplerInfo.MinificationFilter));
	}

	void SamplerInstanceGL::destroy() {
		glDeleteSamplers(1, &ID);
	}

	void SamplerInstanceGL::bind(TextureUnitType unit) const {
		glBindSampler(1, unit);
	}

}
