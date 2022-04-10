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
        m_pos += size;
        return m_bufferData.data() + m_pos;
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

}