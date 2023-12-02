#pragma once

#include "spr\ResourceManager\UniformType.h"
#include "spr\ResourceManager\VertexAttributeLayout.h"

#include <glad/glad.h>

namespace spr {
	UniformType getSPRUniformTypeFromGLType(GLenum type);
	GLenum getGLTypeFromAttributeType(AttributeType attributeType);
	GLenum getBoolGL(bool value);

	int getAttributeLocationCountGL(GLenum attributeType);
	int getAttributeElementCountGL(GLenum attributeType);
}