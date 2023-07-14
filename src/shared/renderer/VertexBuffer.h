#pragma once

#include "Handles.h"
#include "VertexAttributeLayout.h"


namespace spr {
    struct VertexBufferInstanceGL;
}

namespace spr {

    VertexBufferHandle createVertexBuffer(const void* data, uint32_t size, const VertexAttributeLayout& layout);
    void setVertexBuffer(const VertexBufferHandle& handle);
    void destroy(VertexBufferHandle& handle);

}

namespace spr { namespace internal {
    
    const VertexBufferInstanceGL& getVertexBuffer(const VertexBufferHandle& handle);
    void bindVertexBuffer(const VertexBufferHandle& handle);

}}

namespace spr {

    struct VertexBufferInstanceGL {
        uint32_t ID;
        uint32_t Size;
        VertexAttributeLayoutHandle LayoutHandle;

        void create(const void* data, uint32_t size, const VertexAttributeLayoutHandle& layoutHandle);
        void destroy();
    };

}