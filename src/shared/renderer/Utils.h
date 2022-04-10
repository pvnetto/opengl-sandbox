#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>

#include "Handles.h"

namespace spr {

    /* Generates valid handles, while keeping track of which handles are busy */
    template <typename T, typename Enable = void>
    class HandleGenerator {};

    template <typename T>
    class HandleGenerator <T, std::enable_if_t<is_handle_type<T>::value, void>> {
    public:
        static inline bool m_handles[T::capacity] = { false };

    public:
        static T allocHandle() {
            for(int i = 0; i < T::capacity; i++) {
                if(m_handles[i]) continue;
                m_handles[i] = true; 
                return i;
            }

            return kInvalidHandle;
        }

        static void removeHandle(T& handle) {
            m_handles[handle.idx] = false;
            handle = kInvalidHandle;
        }

    };

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