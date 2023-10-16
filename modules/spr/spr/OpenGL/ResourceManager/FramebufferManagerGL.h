#pragma once

#include "spr/ResourceManager/FramebufferAttachmentType.h"
#include "spr/MathTypes.h"
#include "TextureManagerGL.h"

#include <vector>

namespace spr {

	using FramebufferAttachmentMapGL = std::unordered_map<FramebufferAttachmentType, TextureInstanceGL>;

	struct FramebufferInstanceGL {
		uint32_t ID = 0;

		static FramebufferInstanceGL DefaultFramebufferGL;

		FramebufferInstanceGL() = default;

		void create(const FramebufferAttachmentMapGL &attachments);
		void destroy();
		void bind() const;
		void blit(const FramebufferInstanceGL &destination, const Rect& sourceRect, const Rect& destinationRect, const uint8_t blitMask) const;
	};


	class FramebufferManagerGL {
	public:
		void createFramebuffer(const FramebufferHandle handle, const FramebufferAttachmentMapGL &attachments);
		void destroy(FramebufferHandle &handle);

		const FramebufferInstanceGL &getFramebuffer(const FramebufferHandle handle) const;

	private:
		FramebufferInstanceGL m_Framebuffers[FramebufferHandle::capacity];
	};

}