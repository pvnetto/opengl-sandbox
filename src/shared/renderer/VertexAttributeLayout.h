#pragma once

#include <unordered_map> 

namespace spr {

    enum class AttributeType {
        Int,
        UnsignedInt,
        Float,
    };

    struct VertexAttribute {
        const char* Name;
        AttributeType Type;
        uint32_t Num;
        bool Normalized;
        uint32_t Offset = 0;

        VertexAttribute() = default;
        VertexAttribute(const char* name, AttributeType type, uint32_t num, bool normalized = false);

        uint32_t getAttributeSize() const;
    };

    class VertexAttributeLayout {

    private:
        std::vector<VertexAttribute> m_attributes;

        uint32_t m_stride = 0;
        uint32_t m_hash = 0;

    public:
        VertexAttributeLayout& begin();
        VertexAttributeLayout& add(VertexAttribute attribute);
        VertexAttributeLayout& end();

        inline bool operator==(const VertexAttributeLayout& other) { return m_hash == other.m_hash; }
        inline bool operator!=(const VertexAttributeLayout& other) { return !(*this == other); }

    };

}