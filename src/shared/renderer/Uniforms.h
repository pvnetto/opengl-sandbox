#pragma once

// ========================================
// ==== Uniforms ==========================
// ========================================
namespace spr {

    struct UniformHandle;
    enum class UniformType;

    UniformHandle createUniform(const char* name, UniformType type);
    void setUniform(const UniformHandle& uniformHandle, const void* data);
    void destroy(UniformHandle& uniformHandle);

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