#pragma once

#include "spr/ResourceManager/UniformManager.h"

namespace spr {

	struct VertexBufferInstanceGL;
	struct ShaderInstanceGL;
	class UniformManager;

	using UniformInfoBuffer = Buffer;
	using UniformInfoBufferPtr = std::shared_ptr<UniformInfoBuffer>;

    struct ProgramInstanceGL {
		struct ProgramAttributeGL {
			/* Name of the attribute inside the program */
			std::string Name;
			/* Attribute location inside the program (Note: Attributes may take up multiple contiguous locations, this value stores the base location) */
			int Location;
			/* GL type of the attribute (GLenum) */
			int Type;
		};

		uint32_t ID;
		UniformInfoBufferPtr UniformInfoBuffer;

		mutable std::vector<ProgramAttributeGL> Attributes;
		mutable uint32_t CurrentBufferBindingPoint = -1;

		void create(const ShaderInstanceGL &vertexShader, const ShaderInstanceGL &fragmentShader, const UniformManager &uniformManager);
		void destroy();
		void use() const;

		void resetAttributeBindings() const;
		void bindAttributes(uint32_t vaoId, const VertexBufferInstanceGL& vertexBuffer) const;

		void findUniforms(const UniformManager &uniformManager);
		void findAttributes();
	};

	struct ProgramUniformInfoGL
	{
		uint32_t Location;
		UniformHandle Handle;
		uint8_t Index;
		UniformType Type;
	};

	class ProgramManagerGL {
	public:
		void createProgram(const ProgramHandle &handle, const ShaderInstanceGL &vertexShader, const ShaderInstanceGL &fragmentShader, const UniformManager& uniformManager);
		void destroy(ProgramHandle &programHandle);

		const ProgramInstanceGL &getProgram(const ProgramHandle &handle) const;

	private:
		ProgramInstanceGL m_Programs[ProgramHandle::capacity];
	};

}