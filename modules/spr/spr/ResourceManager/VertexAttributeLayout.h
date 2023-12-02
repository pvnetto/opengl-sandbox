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
		/* Name of the attribute. Should match the name in the shader */
		std::string Name = "";
		/* Type of the attribute */
		AttributeType Type = AttributeType::Float;
		/* Number of elements in the attribute (e.g., Num = 4 with AttributeType = Float would mean we have 4 floats per attribute) */
		uint32_t Num = 0;
		/* Determines whether the attribute should be normalized */
		bool Normalized = false;
		/* Offset (given in bytes) between the start of the buffer and the first value of the attribute */
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
		bool getAttribute(int index, VertexAttribute& outAttribute) const;
		bool getAttribute(const std::string &name, VertexAttribute& outAttribute) const;

        inline bool operator==(const VertexAttributeLayout& other) const { return m_Hash == other.m_Hash; }
        inline bool operator!=(const VertexAttributeLayout& other) const { return !(*this == other); }

    };

}
