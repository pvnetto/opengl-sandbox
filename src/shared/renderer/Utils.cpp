#include "Utils.h"

#include "assert.h"

namespace spr {

    Buffer::Buffer(uint32_t size) {
        m_bufferData.reserve(size);
    }
    
    BufferPtr Buffer::alloc(uint32_t size) {
        return std::make_shared<Buffer>(size);
    }

    void Buffer::write(const void* data, uint32_t size) {
        bool dataFitsBuffer = m_pos + size < m_bufferData.capacity(); 
        assert(dataFitsBuffer && "::ERROR: Buffer size is not big enough for this data");
        
        if(dataFitsBuffer) {
            void* writeLocationStart = m_bufferData.data() + m_pos;
            memcpy(writeLocationStart, data, size);
            m_pos += size;
        }
    }

    void* Buffer::read(uint32_t size) {
        if(m_pos + size > m_bufferData.capacity()) return nullptr;
        const uint32_t offset = m_pos;
        m_pos += size;
        return m_bufferData.data() + offset;
    }
    
    // Updates buffer size when remaining space is smaller than a certain threshold
    void Buffer::update() {
        static const uint32_t resizeThreshold = 65536;
        static const uint32_t growthPerResize = 1 << 20;

        const uint32_t remainingSpace = m_bufferData.capacity() - m_pos;
        if(remainingSpace <= resizeThreshold) {
            m_bufferData.reserve(m_bufferData.capacity() + growthPerResize);
        }
    }

    #define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

    uint32_t murmurHash2A(const void* key, int len, uint32_t seed)
    {
        const uint32_t m = 0x5bd1e995;
        const int r = 24;
        uint32_t l = len;

        const unsigned char * data = (const unsigned char *)key;

        uint32_t h = seed;

        while(len >= 4)
        {
            uint32_t k = *(uint32_t*)data;

            mmix(h,k);

            data += 4;
            len -= 4;
        }

        uint32_t t = 0;

        switch(len)
        {
            case 3: t ^= data[2] << 16;
            case 2: t ^= data[1] << 8;
            case 1: t ^= data[0];
        };

        mmix(h,t);
        mmix(h,l);

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }


}