#include "FramebufferManagerGL.h"

#include "FramebufferUtilsGL.h"

#include <glad/glad.h>

#include <cassert>

namespace spr {

	FramebufferInstanceGL FramebufferInstanceGL::DefaultFramebufferGL = {};

	void FramebufferInstanceGL::create(const FramebufferAttachmentMapGL &attachments) {
		glCreateFramebuffers(1, &ID);

		for (const auto [attachmentType, attachmentTextureInstance] : attachments) {
			const GLenum attachmentTypeGl = getFramebufferAttachmentTypeGL(attachmentType);
			glNamedFramebufferTexture(ID, attachmentTypeGl, attachmentTextureInstance.ID, 0);
		}

		if (auto status = glCheckNamedFramebufferStatus(ID, GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE) {
			assert(false && "::ERROR: Framebuffer is incomplete");
		}
	}

	void FramebufferInstanceGL::destroy() {
		glDeleteFramebuffers(1, &ID);
	}

	void FramebufferInstanceGL::bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}

	void FramebufferInstanceGL::blit(const FramebufferInstanceGL &destination, const Rect &sourceRect, const Rect &destinationRect, const uint8_t blitMask) const {
		const GLbitfield blitMaskGL = getFramebufferAttachmentFlagsGL(blitMask);
		glBlitNamedFramebuffer(ID, destination.ID,
			sourceRect.OffsetX, sourceRect.OffsetY, sourceRect.OffsetX + sourceRect.Width, sourceRect.OffsetY + sourceRect.Height,
			destinationRect.OffsetX, destinationRect.OffsetY, destinationRect.OffsetX + destinationRect.Width, destinationRect.OffsetY + destinationRect.Height,
		    blitMaskGL, GL_LINEAR);
	}

	void FramebufferManagerGL::createFramebuffer(const FramebufferHandle handle, const FramebufferAttachmentMapGL &attachments) {
		m_Framebuffers[handle.idx].create(attachments);
	}

	void FramebufferManagerGL::destroy(FramebufferHandle &handle) {
		m_Framebuffers[handle.idx].destroy();
	}
	const FramebufferInstanceGL &FramebufferManagerGL::getFramebuffer(const FramebufferHandle handle) const {
		if (handle.isValid()) {
			return m_Framebuffers[handle.idx];
		}

		return FramebufferInstanceGL::DefaultFramebufferGL;
	}
}

