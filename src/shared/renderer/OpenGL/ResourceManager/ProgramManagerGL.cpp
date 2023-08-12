#include "ProgramManagerGL.h"

#include "shared/renderer/ResourceManager/VertexAttributeLayout.h"
#include "shared/renderer/ResourceManager/UniformManager.h"
#include "shared/renderer/OpenGL/ResourceManager/ShaderManagerGL.h"
#include "shared/renderer/OpenGL/ResourceManager/VertexBufferManagerGL.h"

#include <iostream>
#include <glad/glad.h>

namespace spr {

	void ProgramManagerGL::createProgram(const ProgramHandle &handle, const ShaderInstanceGL &vertexShader, const ShaderInstanceGL &fragmentShader, const UniformManager &uniformManager) {
		assert(handle.isValid() && vertexShader.ID != 0 && fragmentShader.ID != 0 && "::ERROR: Invalid shader");
		m_Programs[handle.idx].create(vertexShader, fragmentShader, uniformManager);
	}

	void ProgramManagerGL::destroy(ProgramHandle &programHandle) {
		m_Programs[programHandle.idx].destroy();
	}

	const ProgramInstanceGL& ProgramManagerGL::getProgram(const ProgramHandle &handle) const {
		assert(handle.isValid() && "::ERROR: Invalid program");
		return m_Programs[handle.idx];
	}

	namespace {
		UniformType getSPRUniformTypeFromGLType(GLenum type);
		GLenum getGLTypeFromAttributeType(AttributeType attributeType);
		GLenum getGLBool(bool value);
	}

    void ProgramInstanceGL::create(const ShaderInstanceGL &vertexShader, const ShaderInstanceGL &fragmentShader, const UniformManager& uniformManager) {
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader.ID);
		glAttachShader(ID, fragmentShader.ID);
		glLinkProgram(ID);

		int linkSuccess;
		glGetProgramiv(ID, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess) {
			char linkInfo[512];
			glGetProgramInfoLog(ID, 512, NULL, linkInfo);
			std::cout << "::ERROR: Failed to link shader " << linkInfo << "\n";
		}

		if (!UniformInfoBuffer) {
			UniformInfoBuffer = UniformInfoBuffer::alloc(1024);
		}

		findUniforms(uniformManager);
		findAttributes();
	}

	void ProgramInstanceGL::destroy() {
		glDeleteProgram(ID);
		ID = 0;
		UniformInfoBuffer = nullptr;
	}

	void ProgramInstanceGL::use() const {
		glUseProgram(ID);
	}

	void ProgramInstanceGL::bindAttributes(uint32_t vaoId, const VertexBufferInstanceGL &vertexBuffer) const {
		// TODO: Check ARB_vertex_attrib_binding extension
		// TODO: Alternatively, we could have one VAO per attribute layout, and only switch buffers when required 
		VertexAttributeLayout &layout = spr::getVertexAttributeLayout(vertexBuffer.LayoutHandle);
		static constexpr uint32_t bufferBindingPoint = 0;
		glVertexArrayVertexBuffer(vaoId, bufferBindingPoint, vertexBuffer.ID, NULL, layout.getStride());
		for (int i = 0; i < Attributes.size(); i++) {
			const VertexAttribute &layoutAttribute = layout.getAttribute(i);
			const ProgramAttributeGL &programAttribute = Attributes[i];

			assert(layoutAttribute.Name == programAttribute.Name && "::ERROR: Vertex layout attribute location mismatch");
			glVertexArrayAttribFormat(vaoId, i,
			                          layoutAttribute.Num,
			                          getGLTypeFromAttributeType(layoutAttribute.Type),
			                          getGLBool(layoutAttribute.Normalized),
			                          layoutAttribute.Offset);
			glEnableVertexArrayAttrib(vaoId, i);
			glVertexArrayAttribBinding(vaoId, i, bufferBindingPoint);
		}
	}
	
	void ProgramInstanceGL::findUniforms(const UniformManager& uniformManager) {
		UniformInfoBuffer->reset();

		GLint activeUniformCount, maxUniformNameLength;
		glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &activeUniformCount);
		glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

		std::string name;
		name.reserve(maxUniformNameLength);
		for (int i = 0; i < activeUniformCount; i++) {
			GLenum type;
			GLint count;
			name.clear();

			glGetActiveUniform(ID, i, maxUniformNameLength, NULL, &count, &type, name.data());
			uint32_t location = glGetUniformLocation(ID, name.c_str());

			const UniformHandle handle = uniformManager.getUniformByName(name.c_str());
			const UniformType sprType = getSPRUniformTypeFromGLType(type);

			UniformInfoBuffer->write(&sprType, sizeof(UniformType));
			UniformInfoBuffer->write(&location, sizeof(uint32_t));
			UniformInfoBuffer->write(&handle, sizeof(UniformHandle));
		}
	}

	void ProgramInstanceGL::findAttributes() {
		Attributes.clear();

		GLint activeAttributesCount, maxAttributeNameLength;
		glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTES, &activeAttributesCount);
		Attributes.resize(activeAttributesCount);

		glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
		std::string name;
		name.reserve(maxAttributeNameLength);
		for (int i = 0; i < activeAttributesCount; i++) {
			GLenum type;
			GLint count;
			name.clear();

			glGetActiveAttrib(ID, i, maxAttributeNameLength, nullptr, &count, &type, name.data());
			int32_t location = glGetAttribLocation(ID, name.c_str());
			Attributes[i].Name = name.c_str();
			Attributes[i].Location = location;
		}
	}

	namespace {

		UniformType getSPRUniformTypeFromGLType(GLenum type) {
			switch (type) {
			case GL_INT:
				return UniformType::Integer;
			case GL_FLOAT:
				return UniformType::Float;
			case GL_FLOAT_VEC2:
				return UniformType::Vec2;
			case GL_FLOAT_VEC3:
				return UniformType::Vec3;
			case GL_SAMPLER_2D:
				return UniformType::Sampler;
			case GL_FLOAT_MAT4:
				return UniformType::Mat4x4;
			default:
				assert(false && "::ERROR: Undefined uniform type");
				return UniformType::Float;
			}
		}

		GLenum getGLTypeFromAttributeType(AttributeType attributeType) {
			switch (attributeType) {
			case AttributeType::Float:
				return GL_FLOAT;
			case AttributeType::Int:
				return GL_INT;
			case AttributeType::UnsignedInt:
				return GL_UNSIGNED_INT;
			default:
				assert(false && "::ERROR: Undefined attribute type");
				return GL_FLOAT;
			}
		}

		GLenum getGLBool(bool value) {
			return value ? GL_TRUE : GL_FALSE;
		}

	}

}