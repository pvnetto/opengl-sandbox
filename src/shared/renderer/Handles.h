#pragma once

#include <cstdint>

namespace spr {
    
    using HandleType = uint16_t;

    static const HandleType kInvalidHandle = UINT16_MAX;

#define SPR_DEFINE_HANDLE(name) \
	struct name##Handle { \
        uint16_t idx;                                                 \
	    inline bool isValid() const { return spr::kInvalidHandle != idx; }  \
    };
}


namespace spr {

    SPR_DEFINE_HANDLE(VertexBuffer)
    SPR_DEFINE_HANDLE(IndexBuffer)
    SPR_DEFINE_HANDLE(VertexLayout)

    SPR_DEFINE_HANDLE(Shader)
    SPR_DEFINE_HANDLE(Program)
    SPR_DEFINE_HANDLE(Uniform)

}