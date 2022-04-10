#pragma once

#include <cstdint>
#include <type_traits>

namespace spr {

#define SPR_MAX_UNIFORM_COUNT 512
#define SPR_MAX_PROGRAMS 256
#define SPR_MAX_SHADERS 512

}

namespace spr {
    
    using HandleType = uint16_t;

    static const HandleType kInvalidHandle = UINT16_MAX;

    template<class T>
    struct is_handle_type : std::false_type {};

#define SPR_DEFINE_HANDLE(name, cap) \
	struct name##Handle { \
        static constexpr uint32_t capacity = cap;                     \
        uint16_t idx;                                                 \
	    inline bool isValid() const { return spr::kInvalidHandle != idx; }  \
    }; \
    template<> struct is_handle_type<name##Handle> : std::true_type {}; 

}


namespace spr {

    SPR_DEFINE_HANDLE(Shader, SPR_MAX_SHADERS)
    SPR_DEFINE_HANDLE(Program, SPR_MAX_PROGRAMS)
    SPR_DEFINE_HANDLE(Uniform, SPR_MAX_UNIFORM_COUNT)

}