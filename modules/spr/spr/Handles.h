#pragma once

#include <cstdint>
#include <type_traits>

namespace spr {

#define SPR_MAX_UNIFORM_COUNT 512
#define SPR_MAX_PROGRAMS_COUNT 256
#define SPR_MAX_SHADERS_COUNT 512
#define SPR_MAX_TEXTURES_COUNT 256
#define SPR_MAX_VERTEX_BUFFER_COUNT 256
#define SPR_MAX_INDEX_BUFFER_COUNT 256
#define SPR_MAX_FRAMEBUFFER_COUNT 64
#define SPR_MAX_ATTRIBUTE_LAYOUT_COUNT 32

}

namespace spr {
    
    using HandleType = uint16_t;

    static inline const HandleType kInvalidHandle = UINT16_MAX;

    template<class T>
    struct is_handle_type : std::false_type {};

#define SPR_DEFINE_HANDLE(name, cap) \
    struct name##Handle { \
        static constexpr uint32_t capacity = cap;                           \
        uint16_t idx;                                                       \
        name##Handle() : idx(kInvalidHandle) {};                            \
        name##Handle(uint32_t handleIdx) : idx(handleIdx) {}                \
	inline bool isValid() const { return spr::kInvalidHandle != idx; }  \
        inline bool operator==(const name##Handle& other) const { return idx == other.idx; }  \
        inline bool operator!=(const name##Handle& other) const { return !(*this == other); } \
    }; \
    template<> struct is_handle_type<name##Handle> : std::true_type {}; 

}


namespace spr {

    SPR_DEFINE_HANDLE(Shader, SPR_MAX_SHADERS_COUNT)
    SPR_DEFINE_HANDLE(Program, SPR_MAX_PROGRAMS_COUNT)
    SPR_DEFINE_HANDLE(Uniform, SPR_MAX_UNIFORM_COUNT)
	SPR_DEFINE_HANDLE(Texture, SPR_MAX_TEXTURES_COUNT)
    SPR_DEFINE_HANDLE(VertexBuffer, SPR_MAX_VERTEX_BUFFER_COUNT)
    SPR_DEFINE_HANDLE(IndexBuffer, SPR_MAX_INDEX_BUFFER_COUNT)
    SPR_DEFINE_HANDLE(Framebuffer, SPR_MAX_FRAMEBUFFER_COUNT)
    SPR_DEFINE_HANDLE(VertexAttributeLayout, SPR_MAX_ATTRIBUTE_LAYOUT_COUNT)

}
