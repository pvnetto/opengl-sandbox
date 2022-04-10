#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace spr {

    /* Buffer data container. */
    class Buffer;
    using BufferPtr = std::shared_ptr<Buffer>;
    class Buffer {
    private:
        // Read/write operations will start from this position in the buffer
        uint32_t m_pos = 0;
        std::vector<char> m_bufferData;

    public:
        Buffer(uint32_t size);

        void write(const void* data, uint32_t size);
        void* read(uint32_t size);

        template <typename T>
        T read() {
            if(void* value = read(sizeof(T))) {
                return *(static_cast<T*>(value));
            }
            return {};
        }

        // Updates buffer size when remaining memory is smaller than a certain threshold
        void update();

        inline void reset(uint32_t newPos = 0) { m_pos = newPos; }
        inline uint32_t getPos() { return m_pos; }

    public:
        static BufferPtr alloc(uint32_t size = 1 << 20);
    };

}