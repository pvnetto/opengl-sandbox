#include "FramebufferUtilsGL.h"

#include <cstdint>
#include <cassert>

namespace spr {

	GLenum getFramebufferAttachmentTypeGL(FramebufferAttachmentType attachmentType) {
		if (attachmentType > FramebufferAttachmentType::ColorMIN && attachmentType < FramebufferAttachmentType::ColorMAX) {
			const uint8_t colorAttachmentIndex = ((uint8_t)attachmentType - (uint8_t)FramebufferAttachmentType::ColorMIN) - 1;
			return GL_COLOR_ATTACHMENT0 + colorAttachmentIndex;
		}
		else if (attachmentType == FramebufferAttachmentType::Depth) {
			return GL_DEPTH_ATTACHMENT;
		}
		else if (attachmentType == FramebufferAttachmentType::Stencil) {
			return GL_STENCIL_ATTACHMENT;
		}
		else if (attachmentType == FramebufferAttachmentType::DepthStencil) {
			return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		assert(false && "spr::ERROR: This attachment type is not supported.");
		return GL_COLOR_ATTACHMENT0;
	}

	GLbitfield getFramebufferAttachmentFlagsGL(const uint8_t attachmentFlags) {
		GLbitfield attachmentFlagsGL = 0;

		if (HasFlag(attachmentFlags, FramebufferAttachmentFlags::Color)) {
			attachmentFlagsGL |= GL_COLOR_BUFFER_BIT;
		}
		if (HasFlag(attachmentFlags, FramebufferAttachmentFlags::Depth)) {
			attachmentFlagsGL |= GL_DEPTH_BUFFER_BIT;
		}
		if (HasFlag(attachmentFlags, FramebufferAttachmentFlags::Stencil)) {
			attachmentFlagsGL |= GL_STENCIL_BUFFER_BIT;
		}

		return attachmentFlagsGL;
	}

	GLbitfield getClearFlagsGL(const uint8_t clearFlags) {
		return getFramebufferAttachmentFlagsGL(clearFlags);
	}

}