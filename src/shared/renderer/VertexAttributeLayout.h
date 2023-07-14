#pragma once

#include "Handles.h"

#include <vector>
#include <string>
#include <unordered_map> 

namespace spr {
    class VertexAttributeLayout;
    VertexAttributeLayoutHandle findOrCreateVertexAttributeLayout(const VertexAttributeLayout& layout);
    VertexAttributeLayout& getVertexAttributeLayout(const VertexAttributeLayoutHandle& handle);
}

namespace spr {

    enum class AttributeType {
        Int,
        UnsignedInt,
        Float,
    };

    struct VertexAttribute {
        std::string Name;
        AttributeType Type;
        uint32_t Num;
        bool Normalized;
        uint32_t Offset = 0;

        VertexAttribute() = default;
        VertexAttribute(std::string name, AttributeType type, uint32_t num, bool normalized = false);

        uint32_t getAttributeSize() const;
    };

    class VertexAttributeLayout {

    private:
        std::vector<VertexAttribute> m_attributes;

        uint32_t m_stride = 0;
        uint32_t m_hash = 0;

    public:
        inline uint32_t getStride() const { return m_stride; }

        VertexAttributeLayout& begin();
        VertexAttributeLayout& add(VertexAttribute attribute);
        VertexAttributeLayout& end();

        const VertexAttribute& getAttribute(int index) const;

        inline bool operator==(const VertexAttributeLayout& other) const { return m_hash == other.m_hash; }
        inline bool operator!=(const VertexAttributeLayout& other) const { return !(*this == other); }

    };

}
