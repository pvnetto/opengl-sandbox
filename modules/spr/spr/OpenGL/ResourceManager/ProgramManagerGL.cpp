#include "ProgramManagerGL.h"

#include "spr/ResourceManager/VertexAttributeLayout.h"
#include "spr/ResourceManager/UniformManager.h"
#include "spr/OpenGL/ResourceManager/ShaderManagerGL.h"
#include "spr/OpenGL/ResourceManager/VertexBufferManagerGL.h"

#include <iostream>
#include <glad/glad.h>
#include "ProgramUtilsGL.h"

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
			assert(false);
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

	void ProgramInstanceGL::resetAttributeBindings() const {
		CurrentBufferBindingPoint = 0;
	}

	void ProgramInstanceGL::bindAttributes(uint32_t vaoId, const VertexBufferInstanceGL &vertexBuffer) const {
		// TODO: Check ARB_vertex_attrib_binding extension
		// TODO: Alternatively, we could have one VAO per attribute layout, and only switch buffers when required
		VertexAttributeLayout &layout = spr::getVertexAttributeLayout(vertexBuffer.LayoutHandle);
		glVertexArrayVertexBuffer(vaoId, CurrentBufferBindingPoint, vertexBuffer.ID, NULL, layout.getStride());
		for (int i = 0; i < Attributes.size(); i++) {
			const ProgramAttributeGL &programAttribute = Attributes[i];
			const int attributeBaseLocation = programAttribute.Location;
			const std::string &attributeName = programAttribute.Name;

			VertexAttribute layoutAttribute;
			if (!layout.getAttribute(attributeName, layoutAttribute)) {
				continue;
			}

			// Attributes can only have up to 4 elements at a time. GLSL solves this by assigning more than one location for attributes that require more than 4 elements.
			// e.g., mat4x4 takes up 4 locations, each with 4 elements per attribute.
			const int locationCount = getAttributeLocationCountGL(programAttribute.Type);
			const int attributeElementCount = layoutAttribute.Num / locationCount;
			const GLenum attributeType = getGLTypeFromAttributeType(layoutAttribute.Type);
			const GLboolean bIsAttributeNormalized = getBoolGL(layoutAttribute.Normalized);
			const int attributeStride = layout.getStride() / locationCount;
			const int attributeOffset = layoutAttribute.Offset;
			assert(layoutAttribute.Num == getAttributeElementCountGL(programAttribute.Type));
			for (int currentLocation = attributeBaseLocation; currentLocation < attributeBaseLocation + locationCount; currentLocation++) {
				// Attributes that take up more than one location and live in the same buffer are offset from each other
				const int relativeLocation = currentLocation - attributeBaseLocation;
				const int additionalOffset = relativeLocation * attributeStride;

				glVertexArrayAttribFormat(vaoId, currentLocation,
				                          attributeElementCount,
										  attributeType,
				                          bIsAttributeNormalized,
										  attributeOffset + additionalOffset);
				glEnableVertexArrayAttrib(vaoId, currentLocation);
				glVertexArrayAttribBinding(vaoId, currentLocation, CurrentBufferBindingPoint);			
			}
		}

		if (vertexBuffer.InstanceCount > 0) {
			const int size = vertexBuffer.ByteSize / layout.getStride();
			const int divisor = size / vertexBuffer.InstanceCount;
			assert(divisor > 0);
			glVertexArrayBindingDivisor(vaoId, CurrentBufferBindingPoint, divisor);
		}
		CurrentBufferBindingPoint++;
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

			// TODO: This probably doesn't work as intended on arrays!!! When count > 1, we need to store count on ProgramUniformInfoGL and set the correct count in RendererContextGL
			for (int i = 0; i < count; i++) {
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

		// We'll use this mask to check whether an attribute is taking a location that was already taken by another attribute.
		// e.g. Let's say we have a mat4x4 attribute at location = 4, it would take one location for each row, so locations 4, 5, 6, 7 would all be busy,
		// and while we'd still be able to assign an attribute to location = 5 on GLSL, it would be wrong. This is what we're trying to catch here.
		uint32_t attributeLocationMask = 0;

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
			Attributes[i].Type = type;

			const int locationCount = getAttributeLocationCountGL(type);
			for (int currentLocation = location; currentLocation < location + locationCount; currentLocation++) {
				const bool bIsLocationTaken = (attributeLocationMask >> currentLocation) & 1;
				assert(!bIsLocationTaken && "::ERROR: Attribute location is already taken");
				attributeLocationMask |= (1 << currentLocation);
			}
		}
	}

}