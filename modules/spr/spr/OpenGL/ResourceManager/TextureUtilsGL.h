#pragma once

#include "spr/ResourceManager/TextureInfo.h"
#include "spr/ResourceManager/SamplerInfo.h"

#include <glad/glad.h>

namespace spr {
	GLenum getInternalFormatGL(spr::TextureFormat format);
	GLenum getFormatGL(spr::TextureFormat format);

	GLenum getFilterMethodGL(spr::FilterMethod filterMethod);
	GLenum getWrappingMethodGL(spr::WrappingMethod wrappingMethod);
}