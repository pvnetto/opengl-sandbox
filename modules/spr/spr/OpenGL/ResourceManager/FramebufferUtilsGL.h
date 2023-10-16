#pragma once

#include "spr/ResourceManager/FramebufferAttachmentType.h"

#include <glad/glad.h>

namespace spr {

	GLenum getFramebufferAttachmentTypeGL(FramebufferAttachmentType attachmentType);
	GLbitfield getFramebufferAttachmentFlagsGL(const uint8_t attachmentFlags);
	GLbitfield getClearFlagsGL(const uint8_t clearFlags);

}