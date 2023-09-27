#include "VertexAttributeLayout.h"
#include "spr/Utils.h"

#include <assert.h>


namespace spr {
    static constexpr uint32_t s_NumLayouts = VertexAttributeLayoutHandle::capacity;
	static HandleGenerator<VertexAttributeLayoutHandle> s_AttributeLayoutHandles;
    static VertexAttributeLayout s_VertexAttributeLayouts[s_NumLayouts];

    VertexAttributeLayoutHandle findOrCreateVertexAttributeLayout(const VertexAttributeLayout& layout) {
        for(int i = 0; i < s_NumLayouts; i++) {
            if(layout == s_VertexAttributeLayouts[i]) {
                return i;
            }
        }

        VertexAttributeLayoutHandle layoutHandle = s_AttributeLayoutHandles.allocHandle();
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
        m_Attributes.clear();
        m_Stride = 0;
        m_Hash = 0;
        return *this;
    }

    VertexAttributeLayout& VertexAttributeLayout::add(VertexAttribute attribute) {
        attribute.Offset = m_Stride;
        m_Attributes.push_back(attribute);
        m_Stride += attribute.getAttributeSize();
        return *this;
    }

    VertexAttributeLayout& VertexAttributeLayout::end() {
        std::size_t hash = 0;
        hashCombine(hash, murmurHash2A(&m_Stride, sizeof(m_Stride)));

        for (const auto &attribute : m_Attributes) {
			hashCombine(hash, attribute.getHash());
        }
        
        m_Hash = hash;

        return *this;
    }

    const VertexAttribute& VertexAttributeLayout::getAttribute(int index) const {

        if(index >= 0 && index < m_Attributes.size()) {
            return m_Attributes[index];
        }

        static VertexAttribute empty; 
        assert(false && "::ERROR: Invalid attribute index");
        return empty;
    }

    VertexAttribute::VertexAttribute(const std::string& name, AttributeType type, uint32_t num, bool normalized) :
        Name(name), Type(type), Num(num), Normalized(normalized) {}

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

    uint32_t VertexAttribute::getHash() const {
		std::size_t hash = 0;

		hashCombine(hash,
		            murmurHash2A(Name.c_str(), Name.size()),
		            murmurHash2A(&Type, sizeof(AttributeType)),
		            murmurHash2A(&Num, sizeof(uint32_t)),
		            murmurHash2A(&Normalized, sizeof(bool)),
		            murmurHash2A(&Offset, sizeof(uint32_t)));
		return hash;
    }

}