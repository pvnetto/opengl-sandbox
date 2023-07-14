#include "Programs.h"
#include "VertexAttributeLayout.h"

#include <iostream>
#include <glad/glad.h>
#include <assert.h>
#include <unordered_map>


// ========================================
// ==== Shaders ===========================
// ========================================

namespace spr {

    struct ShaderInstanceGL {
        uint32_t ID;

        void create(unsigned int shaderType, const char* shaderSrc);
        void destroy();
    };

    struct ProgramInstanceGL {
        struct ProgramAttributeGL {
            std::string Name;
            int Location;
        };
        
        uint32_t ID;
        UniformInfoBufferPtr UniformInfoBuffer;
        std::vector<ProgramAttributeGL> Attributes;

        void create(const ShaderInstanceGL& vertexShader, const ShaderInstanceGL& fragmentShader);
        void findUniforms();
        void findAttributes();
        void bindVertexAttributeLayout(const VertexAttributeLayoutHandle& layoutHandle);
        void destroy();
    };

    static ShaderInstanceGL s_Shaders[ShaderHandle::capacity];
    static ProgramInstanceGL s_Programs[ProgramHandle::capacity];

}

namespace spr {

    static GLuint getGLShaderTypeFromSPR(unsigned int sprShaderType) {
        if(sprShaderType == SPR_VERTEX_SHADER) return GL_VERTEX_SHADER;
        if(sprShaderType == SPR_FRAGMENT_SHADER) return GL_FRAGMENT_SHADER;

        assert(false && "::ERROR: Shader type is not supported");
        return GL_VERTEX_SHADER;
    }
    
    ShaderHandle createShader(unsigned int shaderType, const char* shaderSrc) {
        ShaderHandle shaderHandle = HandleGenerator<ShaderHandle>::allocHandle();
        s_Shaders[shaderHandle.idx].create(shaderType, shaderSrc);
        return shaderHandle;
    }

    void destroy(ShaderHandle& shaderHandle) {
        s_Shaders[shaderHandle.idx].destroy();
        HandleGenerator<ShaderHandle>::removeHandle(shaderHandle);
    }
}


// ========================================
// ==== Programs ==========================
// ========================================
namespace spr {

    ProgramHandle createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders) {
        assert((vertexHandle.isValid() && fragmentHandle.isValid()) && "::ERROR: Invalid shader");
        ProgramHandle programHandle = HandleGenerator<ProgramHandle>::allocHandle();
        s_Programs[programHandle.idx].create(
            s_Shaders[vertexHandle.idx],
            s_Shaders[fragmentHandle.idx]);

        if(destroyShaders) {
            destroy(vertexHandle);
            destroy(fragmentHandle);
        }

        return programHandle;
    }

    void destroy(ProgramHandle& handle) {
        assert((handle.isValid() && handle.idx < ProgramHandle::capacity) && "::ERROR: Invalid program");
        s_Programs[handle.idx].destroy();
        HandleGenerator<ProgramHandle>::removeHandle(handle);
    }

    void useProgram(const ProgramHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid program");
        glUseProgram(s_Programs[handle.idx].ID);
    }

    UniformInfoBufferPtr getProgramUniforms(const ProgramHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid program");
        return s_Programs[handle.idx].UniformInfoBuffer;
    }

    ProgramInstanceGL& getProgramInstanceGL(const ProgramHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid program");
        return s_Programs[handle.idx];
    }

}

namespace spr { namespace internal {
    void bindVertexAttributeLayout(const ProgramHandle& programHandle, const VertexAttributeLayoutHandle& layoutHandle) {
        assert(programHandle.isValid() && "::ERROR: Invalid program");
        s_Programs[programHandle.idx].bindVertexAttributeLayout(layoutHandle);
    }
}}

namespace spr {

    void ShaderInstanceGL::create(unsigned int shaderType, const char* shaderSrc) {
        unsigned int glShaderType = getGLShaderTypeFromSPR(shaderType);

        ID = glCreateShader(glShaderType);
        glShaderSource(ID, 1, &shaderSrc, NULL);
        glCompileShader(ID);

        int success;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if(!success) {
            char info[512];
            glGetShaderInfoLog(ID, 512, NULL, info);
            std::cout << "::ERROR: Failed to compile shader:: " << info << "\n";
        }
    }

    void ShaderInstanceGL::destroy() {
        glDeleteShader(ID);
        ID = 0;
    }
    
    void ProgramInstanceGL::create(const ShaderInstanceGL& vertexShader, const ShaderInstanceGL& fragmentShader) {
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader.ID);
        glAttachShader(ID, fragmentShader.ID);
        glLinkProgram(ID);

        int linkSuccess;
        glGetProgramiv(ID, GL_LINK_STATUS, &linkSuccess);
        if(!linkSuccess) {
            char linkInfo[512];
            glGetProgramInfoLog(ID, 512, NULL, linkInfo);
            std::cout << "::ERROR: Failed to link shader " << linkInfo << "\n";
        }

        if(!UniformInfoBuffer) {
            UniformInfoBuffer = UniformInfoBuffer::alloc(1024);
        }

        findUniforms();
        findAttributes();
    }

    static UniformType getSPRUniformTypeFromGLType(GLenum type) {
        switch(type) {
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
            default:
                assert(false && "::ERROR: Undefined uniform type");
                return UniformType::Float;
        }
    }

    void ProgramInstanceGL::findUniforms() {
        UniformInfoBuffer->reset();

        GLint activeUniformCount, maxUniformNameLength;
        glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &activeUniformCount);
        glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

        std::string name;
        name.reserve(maxUniformNameLength);
        for(int i = 0; i < activeUniformCount; i++) {
            GLenum type;
            GLint count;
            name.clear();

            glGetActiveUniform(ID, i, maxUniformNameLength, NULL, &count, &type, name.data());
            uint32_t location = glGetUniformLocation(ID, name.c_str());

            const UniformHandle handle = getUniformByName(name.c_str());
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
        for(int i = 0; i < activeAttributesCount; i++) {
            GLenum type;
            GLint count;
            name.clear();

            glGetActiveAttrib(ID, i, maxAttributeNameLength, nullptr, &count, &type, name.data());
            int32_t location = glGetAttribLocation(ID, name.c_str());
            Attributes[i].Name = name.c_str();
            Attributes[i].Location = location;
        }
    }

    static GLenum getGLTypeFromAttributeType(AttributeType attributeType) {
        switch(attributeType) {
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

    static GLenum getGLBool(bool value) {
        return value ? GL_TRUE : GL_FALSE;
    }

    // Assumes list of program attributes has the same order as layout attributes
    void ProgramInstanceGL::bindVertexAttributeLayout(const VertexAttributeLayoutHandle& layoutHandle) {
        VertexAttributeLayout& layout = spr::getVertexAttributeLayout(layoutHandle);
        for(int i = 0; i < Attributes.size(); i++) {
            const VertexAttribute& layoutAttribute = layout.getAttribute(i);
            const ProgramAttributeGL& programAttribute = Attributes[i];

            assert(layoutAttribute.Name == programAttribute.Name && "::ERROR: Vertex layout attribute location mismatch");

            glVertexAttribPointer(programAttribute.Location,
                layoutAttribute.Num,
                getGLTypeFromAttributeType(layoutAttribute.Type),
                getGLBool(layoutAttribute.Normalized),
                layout.getStride(),
                (void *) (layoutAttribute.Offset));
	        glEnableVertexAttribArray(programAttribute.Location);
        }
    }

    void ProgramInstanceGL::destroy() {
        glDeleteProgram(ID);
        ID = 0;
        UniformInfoBuffer = nullptr;
    }

}