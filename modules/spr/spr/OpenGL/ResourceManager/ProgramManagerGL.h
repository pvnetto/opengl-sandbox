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
			std::string Name;
			int Location;
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