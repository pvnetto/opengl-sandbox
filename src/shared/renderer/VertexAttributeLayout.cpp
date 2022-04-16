#include "VertexAttributeLayout.h"
#include "Utils.h"

#include <assert.h>


namespace spr {
    static constexpr uint32_t s_NumLayouts = VertexAttributeLayoutHandle::capacity;
    static VertexAttributeLayout s_VertexAttributeLayouts[s_NumLayouts];

    VertexAttributeLayoutHandle findOrCreateVertexAttributeLayout(const VertexAttributeLayout& layout) {
        for(int i = 0; i < s_NumLayouts; i++) {
            if(layout == s_VertexAttributeLayouts[i]) {
                return i;
            }
        }

        VertexAttributeLayoutHandle layoutHandle = HandleGenerator<VertexAttributeLayoutHandle>::allocHandle();
        s_VertexAttributeLayouts[layoutHandle.idx] = layout;
        return layoutHandle;
    }

    VertexAttributeLayout& getVertexAttributeLayout(const VertexAttributeLayoutHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid vertex attribute layout");
        return s_VertexAttributeLayouts[handle.idx];   
    }
}


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

    VertexAttribute& VertexAttributeLayout::getAttribute(int index) {

        if(index >= 0 && index < m_attributes.size()) {
            return m_attributes[index];
        }

        static VertexAttribute empty; 
        assert(false && "::ERROR: Invalid attribute index");
        return empty;
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