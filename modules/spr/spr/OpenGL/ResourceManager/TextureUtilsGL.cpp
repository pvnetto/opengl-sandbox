#include "TextureUtilsGL.h"

#include <cassert>

namespace spr {

	GLenum getInternalFormatGL(spr::TextureFormat format) {
		switch (format) {
		case spr::TextureFormat::RGB8:
			return GL_RGB8;
		case spr::TextureFormat::RGBA8:
			return GL_RGBA8;
		case spr::TextureFormat::RGBA16F:
			return GL_RGBA16F;
		case spr::TextureFormat::RED:
			return GL_R8;
		case spr::TextureFormat::DEPTH_24_STENCIL_8:
			return GL_DEPTH24_STENCIL8;
		default:
			assert(false && "::ERROR: Unknown texture format");
			return GL_RGB8;
		}
	}

	GLenum getFormatGL(spr::TextureFormat format) {
		switch (format) {
		case spr::TextureFormat::RGB8:
			return GL_RGB;
		case spr::TextureFormat::RGBA8:
			return GL_RGBA;
		case spr::TextureFormat::RED:
			return GL_RED;
		default:
			assert(false && "::ERROR: Unknown texture format");
			return GL_RGB;
		}
	}

	GLenum getFilterMethodGL(spr::FilterMethod filterMethod) {
		switch (filterMethod) {
		case spr::FilterMethod::Point:
			return GL_NEAREST;
		case spr::FilterMethod::Linear:
			return GL_LINEAR;
		default:
			assert(false && "::ERROR: Unknown filter method");
			return GL_NEAREST;
		}
	}

	GLenum getWrappingMethodGL(spr::WrappingMethod wrappingMethod) {
		switch (wrappingMethod) {
		case spr::WrappingMethod::Repeat:
			return GL_REPEAT;
		case spr::WrappingMethod::Mirror:
			return GL_MIRRORED_REPEAT;
		case spr::WrappingMethod::Clamp:
			return GL_CLAMP_TO_EDGE;
		case spr::WrappingMethod::Border:
			return GL_CLAMP_TO_BORDER;
		default:
			assert(false && "::ERROR: Unknown wrapping method");
			return GL_REPEAT;
		}
	}

}