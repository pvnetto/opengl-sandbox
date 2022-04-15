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


    //-----------------------------------------------------------------------------
    // MurmurHash2A, by Austin Appleby
    // https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.h
    //-----------------------------------------------------------------------------
    uint32_t murmurHash2A(const void* key, int len, uint32_t seed=0xFF22);

    //-----------------------------------------------------------------------------
    // https://stackoverflow.com/a/38140932
    //-----------------------------------------------------------------------------
    inline void hashCombine(std::size_t& seed) { }

    template <typename T, typename... Rest>
    inline void hashCombine(std::size_t& seed, const T& v, Rest... rest) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        hashCombine(seed, rest...);
    }

}