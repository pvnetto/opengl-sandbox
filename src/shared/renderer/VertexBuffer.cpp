#include "VertexBuffer.h"

#include "Utils.h"
#include "Context.h"

#include <glad/glad.h>

namespace spr {

    VertexBufferInstanceGL s_VertexBuffers[VertexBufferHandle::capacity];

    VertexBufferHandle createVertexBuffer(const void* data, uint32_t size, const VertexAttributeLayout& layout) {
        VertexBufferHandle handle = HandleGenerator<VertexBufferHandle>::allocHandle();

        VertexAttributeLayoutHandle layoutHandle = findOrCreateVertexAttributeLayout(layout);
        s_VertexBuffers[handle.idx].create(data, size, layoutHandle);

        return handle;
    }
    
    void setVertexBuffer(const VertexBufferHandle& handle) {
        DrawCallData& drawCallData = spr::getCurrentDrawCallData();
        assert(!drawCallData.VertexBufferHandle.isValid() && "::ERROR: Vertex buffer was already set for this frame.");
        drawCallData.VertexBufferHandle = handle;
    }

    void destroy(VertexBufferHandle& handle) {
        s_VertexBuffers[handle.idx].destroy();
        HandleGenerator<VertexBufferHandle>::removeHandle(handle);
    }

}

namespace spr { namespace internal {

    const VertexBufferInstanceGL& getVertexBuffer(const VertexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid vertex buffer");
        return s_VertexBuffers[handle.idx];
    }

    void bindVertexBuffer(const VertexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid vertex buffer");
        glBindBuffer(GL_VERTEX_ARRAY, s_VertexBuffers[handle.idx].ID);
    }

}}

namespace spr {

    void VertexBufferInstanceGL::create(const void* data, uint32_t size, const VertexAttributeLayoutHandle& layoutHandle) {
        glCreateBuffers(1, &ID);
        glBufferData(GL_VERTEX_ARRAY, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_VERTEX_ARRAY, 0);
    }

    void VertexBufferInstanceGL::destroy() {
        glDeleteBuffers(1, &ID);
        LayoutHandle = kInvalidHandle;
    }

}