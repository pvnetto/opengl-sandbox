#pragma once

#include <cstdint>
#include <vector>
#include <memory>

// ========================================
// ==== Uniforms ==========================
// ========================================
namespace spr {

    struct UniformHandle;
    enum class UniformType;

    UniformHandle createUniform(const char* name, UniformType type);
    void setUniform(const UniformHandle& uniformHandle, const void* data);
    void updateUniform(uint32_t location, const void* data, uint32_t size);
    void destroyUniform(UniformHandle& uniformHandle);

}

namespace spr {

    /* Buffer data container. */
    class SimpleUniformBuffer;
    using SimpleUniformBufferPtr = std::shared_ptr<SimpleUniformBuffer>;
    class SimpleUniformBuffer {
    private:
        uint32_t m_writePos = 0;
        std::vector<char> m_bufferData;

    public:
        SimpleUniformBuffer(uint32_t size);

        void write(const void* data, uint32_t size);

        // Updates buffer size when remaining memory is smaller than a certain threshold
        void update();
    public:
        // Allocates buffer
        static SimpleUniformBufferPtr alloc(uint32_t size = 1 << 20);
    };

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