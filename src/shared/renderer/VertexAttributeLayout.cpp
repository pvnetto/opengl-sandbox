#include "VertexAttributeLayout.h"
#include "Utils.h"

#include <assert.h>

namespace spr {

    VertexAttributeLayout& VertexAttributeLayout::begin() {
        m_attributes.clear();
        m_stride = 0;
        m_hash = 0;
        return *this;
    }

    VertexAttributeLayout& VertexAttributeLayout::add(VertexAttribute attribute) {
        attribute.Offset = m_stride;
        m_attributes.push_back(attribute);
        m_stride += attribute.getAttributeSize();
        return *this;
    }

    VertexAttributeLayout& VertexAttributeLayout::end() {
        std::size_t hash = 0;
        hashCombine(hash,
            murmurHash2A(&m_stride, sizeof(m_stride)),
            murmurHash2A(m_attributes.data(), m_attributes.size() * sizeof(VertexAttribute)));
        
        m_hash = hash;

        return *this;
    }

    VertexAttribute::VertexAttribute(const char* name, AttributeType type, uint32_t num, bool normalized) :
        Name(name), Type(type), Num(num), Normalized(normalized)  { }

    static uint32_t getAttributeSizeByType(AttributeType type) {
        switch (type)
        {
        case AttributeType::Int:
            return sizeof(int);
        case AttributeType::UnsignedInt:
            return sizeof(unsigned int);
        case AttributeType::Float:
            return sizeof(float);
        default:
            assert(false && "::ERROR: Undefined attribute type");
            return 0;
        }
    }

    uint32_t VertexAttribute::getAttributeSize() const {
        return getAttributeSizeByType(Type) * Num;
    }

}