#include "VertexBuffer.h"

#include "Utils.h"
#include "Context.h"

#include <glad/glad.h>

namespace spr {

    static HandleGenerator<VertexBufferHandle> s_VertexBufferHandles;
    VertexBufferInstanceGL s_VertexBuffers[VertexBufferHandle::capacity];

    VertexBufferHandle createVertexBuffer(const void* data, uint32_t size, const VertexAttributeLayout& layout) {
		VertexBufferHandle handle = s_VertexBufferHandles.allocHandle();

        VertexAttributeLayoutHandle layoutHandle = findOrCreateVertexAttributeLayout(layout);
        s_VertexBuffers[handle.idx].create(data, size, layoutHandle);

        return handle;
    }

    void destroy(VertexBufferHandle& handle) {
        s_VertexBuffers[handle.idx].destroy();
		s_VertexBufferHandles.removeHandle(handle);
    }

}

namespace spr { namespace internal {

    const VertexBufferInstanceGL& getVertexBuffer(const VertexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid vertex buffer");
        return s_VertexBuffers[handle.idx];
    }

    void bindVertexBuffer(const VertexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid vertex buffer");
        glBindBuffer(GL_ARRAY_BUFFER, s_VertexBuffers[handle.idx].ID);
    }

}}

namespace spr {

    void VertexBufferInstanceGL::create(const void* data, uint32_t size, const VertexAttributeLayoutHandle& layoutHandle) {
        Size = size;
        LayoutHandle = layoutHandle;

        glCreateBuffers(1, &ID);
        glNamedBufferData(ID, size, data, GL_STATIC_DRAW);
    }

    void VertexBufferInstanceGL::destroy() {
        glDeleteBuffers(1, &ID);
        LayoutHandle = kInvalidHandle;
    }

}
