#pragma once

#include "Handles.h"

namespace spr {
    struct IndexBufferInstanceGL;
}

namespace spr {

    IndexBufferHandle createIndexBuffer(const void* data, uint32_t size);
    void setIndexBuffer(const IndexBufferHandle& handle);
    void destroy(IndexBufferHandle& handle);

}

namespace spr { namespace internal {

    const IndexBufferInstanceGL& getIndexBuffer(const IndexBufferHandle& handle);
    void bindIndexBuffer(const IndexBufferHandle& handle);

}}

namespace spr {

    struct IndexBufferInstanceGL {
        uint32_t ID;
        uint32_t IndexCount;

        void create(const void* data, uint32_t indexCount);
        void destroy();
    };

}
