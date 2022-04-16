#include "IndexBuffer.h"
#include "Context.h"

#include <glad/glad.h>

namespace spr {

    static IndexBufferInstanceGL s_IndexBuffers[IndexBufferHandle::capacity];

    IndexBufferHandle createIndexBuffer(const void* data, uint32_t indexCount) {
        IndexBufferHandle handle = HandleGenerator<IndexBufferHandle>::allocHandle();
        s_IndexBuffers[handle.idx].create(data, indexCount);
        return handle;
    }

    void setIndexBuffer(const IndexBufferHandle& handle) {
        DrawCallData& DrawCallData = spr::getCurrentDrawCallData();
        assert(!DrawCallData.IndexBufferHandle.isValid() && "::ERROR: Index buffer was already set in this draw call");
        DrawCallData.IndexBufferHandle = handle;
    }

    void destroy(IndexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid index buffer");
        s_IndexBuffers[handle.idx].destroy();
        HandleGenerator<IndexBufferHandle>::removeHandle(handle);
    }

}

namespace spr { namespace internal {

    const IndexBufferInstanceGL& getIndexBuffer(const IndexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid index buffer");
        return s_IndexBuffers[handle.idx];
    }
    
    void bindIndexBuffer(const IndexBufferHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid index buffer");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IndexBuffers[handle.idx].ID);
    }

}}

namespace spr {

    void IndexBufferInstanceGL::create(const void* data, uint32_t size) {
        IndexCount = size / sizeof(unsigned int);

        glCreateBuffers(1, &ID);
        glNamedBufferData(ID, size, data, GL_STATIC_DRAW);
    }

    void IndexBufferInstanceGL::destroy() {
        glDeleteBuffers(1, &ID);
    }

}