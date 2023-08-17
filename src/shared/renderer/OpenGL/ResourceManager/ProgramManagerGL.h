#pragma once

#include "shared/renderer/ResourceManager/UniformManager.h"

namespace spr {

	struct VertexBufferInstanceGL;
	struct ShaderInstanceGL;
	class UniformManager;

    struct ProgramInstanceGL {
		struct ProgramAttributeGL {
			std::string Name;
			int Location;
		};

		uint32_t ID;
		UniformInfoBufferPtr UniformInfoBuffer;
		mutable std::vector<ProgramAttributeGL> Attributes;

		void create(const ShaderInstanceGL &vertexShader, const ShaderInstanceGL &fragmentShader, const UniformManager &uniformManager);
		void destroy();
		void use() const;
		void bindAttributes(uint32_t vaoId, const VertexBufferInstanceGL& vertexBuffer) const;

		void findUniforms(const UniformManager &uniformManager);
		void findAttributes();
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