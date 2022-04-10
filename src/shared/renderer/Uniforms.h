#pragma once

#include "Handles.h"
#include "Utils.h"

namespace spr {

    enum class UniformType;
    template<typename T> class UniformBuffer;
    
    // TODO: Use OOP for this buffer.....It makes no sense not to use it
    using UniformDataBuffer = Buffer;
    using UniformInfoBuffer = Buffer;
    using UniformDataBufferPtr = std::shared_ptr<Buffer>;
    using UniformInfoBufferPtr = std::shared_ptr<Buffer>;
}

// ========================================
// ==== Uniforms ==========================
// ========================================
namespace spr {

    UniformHandle createUniform(const char* name, UniformType type);
    void setUniform(const UniformHandle& uniformHandle, const void* data);
    void destroyUniform(UniformHandle& uniformHandle);

    // Updates uniform data on CPU
    void updateUniforms(UniformDataBufferPtr uniformBuffer, uint32_t start, uint32_t end);
    // Sets uniform data in program
    void rendererSetUniforms(UniformInfoBufferPtr uniformMetadataBuffer);
}

namespace spr {

    enum class UniformType {
        Integer,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat3x3,
        Mat4x4,
    };

}