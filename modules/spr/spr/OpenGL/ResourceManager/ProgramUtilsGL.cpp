#include "ProgramUtilsGL.h"

namespace spr {

	UniformType getSPRUniformTypeFromGLType(GLenum type) {
		switch (type) {
		case GL_INT:
			return UniformType::Integer;
		case GL_FLOAT:
			return UniformType::Float;
		case GL_FLOAT_VEC2:
			return UniformType::Vec2;
		case GL_FLOAT_VEC3:
			return UniformType::Vec3;
		case GL_FLOAT_VEC4:
			return UniformType::Vec4;
		case GL_SAMPLER_2D:
			return UniformType::Sampler;
		case GL_FLOAT_MAT4:
			return UniformType::Mat4x4;
		default:
			assert(false && "::ERROR: Undefined uniform type");
			return UniformType::Float;
		}
	}

	GLenum getGLTypeFromAttributeType(AttributeType attributeType) {
		switch (attributeType) {
		case AttributeType::Float:
			return GL_FLOAT;
		case AttributeType::Int:
			return GL_INT;
		case AttributeType::UnsignedInt:
			return GL_UNSIGNED_INT;
		default:
			assert(false && "::ERROR: Undefined attribute type");
			return GL_FLOAT;
		}
	}

	GLenum getBoolGL(bool value) {
		return value ? GL_TRUE : GL_FALSE;
	}

	int getAttributeLocationCountGL(GLenum attributeType) {
		switch (attributeType) {
		case GL_FLOAT_MAT4:
			return 4;
		case GL_FLOAT_MAT4x3:
			return 4;
		case GL_FLOAT_MAT4x2:
			return 4;
		case GL_FLOAT_MAT3:
			return 3;
		case GL_FLOAT_MAT3x2:
			return 3;
		case GL_FLOAT_MAT2:
			return 2;
		}
		return 1;
	}

	int getAttributeElementCountGL(GLenum attributeType) {
		switch (attributeType) {
		case GL_FLOAT:
		case GL_UNSIGNED_INT:
		case GL_SAMPLER_2D:
			return 1;
		case GL_FLOAT_VEC2:
		    return 2;
		case GL_FLOAT_VEC3:
		    return 3;
		case GL_FLOAT_VEC4:
		    return 4;
		case GL_FLOAT_MAT4:
			return 4 * 4;
		case GL_FLOAT_MAT4x3:
			return 4 * 3;
		case GL_FLOAT_MAT4x2:
			return 4 * 2;
		case GL_FLOAT_MAT3:
			return 3 * 3;
		case GL_FLOAT_MAT3x2:
			return 3 * 2;
		case GL_FLOAT_MAT2:
			return 2 * 2;
		default:
			assert(false && "::ERROR: Unsupported type");
			return 0;
		}
	}

}
