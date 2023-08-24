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

			assert(false && "::ERROR: Failed to link shader");
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
			const ProgramAttributeGL &programAttribute = Attributes[i];
			const int attributeLocation = programAttribute.Location;
			const VertexAttribute &layoutAttribute = layout.getAttribute(attributeLocation);

			assert(layoutAttribute.Name == programAttribute.Name && "::ERROR: Vertex layout attribute location mismatch");
			glVertexArrayAttribFormat(vaoId, attributeLocation,
			                          layoutAttribute.Num,
			                          getGLTypeFromAttributeType(layoutAttribute.Type),
			                          getGLBool(layoutAttribute.Normalized),
			                          layoutAttribute.Offset);
			glEnableVertexArrayAttrib(vaoId, attributeLocation);
			glVertexArrayAttribBinding(vaoId, attributeLocation, bufferBindingPoint);
		}
	}

	static void parseUniformName(const char* inName, std::string& outName, uint8_t& outIndex) {
		outName = inName;
		outIndex = 0;

		std::size_t bracketStart = outName.find_first_of('[');
		std::size_t bracketEnd = outName.find_first_of(']');
		if (bracketStart != std::string::npos && bracketEnd != std::string::npos) {
			std::string indexStr = outName.substr(bracketStart + 1, (bracketEnd - bracketStart - 1));
			outIndex = std::stoi(indexStr);
			outName.erase(bracketStart, indexStr.size() + 2);
		}
	}
	
	void ProgramInstanceGL::findUniforms(const UniformManager& uniformManager) {
		UniformInfoBuffer->reset();

		GLint activeUniformCount, maxUniformNameLength;
		glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &activeUniformCount);
		glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

		char *name = new char[maxUniformNameLength];
		for (int i = 0; i < activeUniformCount; i++) {
			GLenum type;
			GLint count;

			memset(name, 0, maxUniformNameLength);
			glGetActiveUniform(ID, i, maxUniformNameLength, NULL, &count, &type, name);

			std::string ìnternalName = "";
			uint8_t uniformIndex = 0;
			parseUniformName(name, ìnternalName, uniformIndex);

			for (int i = 0; i < count; i++) {
				// Inneficient, we could store a single ProgramUniformInfoGL with Index and Count
				ProgramUniformInfoGL uniformInfo;
				uniformInfo.Location = glGetUniformLocation(ID, name);
				uniformInfo.Handle = uniformManager.getUniformByName(ìnternalName.c_str());
				uniformInfo.Type = getSPRUniformTypeFromGLType(type);
				uniformInfo.Index = uniformIndex + i;

				UniformInfoBuffer->write(&uniformInfo, sizeof(ProgramUniformInfoGL));			
			}
		}
		delete[] name;
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