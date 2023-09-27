#pragma once

#include "spr/Handles.h"

#include <vector>
#include <string>
#include <unordered_map> 

// TODO: Refactor to a ResourceManager
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
		std::string Name = "";
		AttributeType Type = AttributeType::Float;
		uint32_t Num = 0;
		bool Normalized = false;
		uint32_t Offset = 0;

        VertexAttribute() = default;
        VertexAttribute(const std::string& name, AttributeType type, uint32_t num, bool normalized = false);

        uint32_t getAttributeSize() const;
		uint32_t getHash() const;
    };

    class VertexAttributeLayout {

    private:
        std::vector<VertexAttribute> m_Attributes;

        uint32_t m_Stride = 0;
        uint32_t m_Hash = 0;

    public:
        inline uint32_t getStride() const { return m_Stride; }

        VertexAttributeLayout& begin();
        VertexAttributeLayout& add(VertexAttribute attribute);
        VertexAttributeLayout& end();

        inline std::size_t getAttributeCount() const { return m_Attributes.size(); }
        const VertexAttribute& getAttribute(int index) const;


        inline bool operator==(const VertexAttributeLayout& other) const { return m_Hash == other.m_Hash; }
        inline bool operator!=(const VertexAttributeLayout& other) const { return !(*this == other); }

    };

}
