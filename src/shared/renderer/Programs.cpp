#include "Programs.h"
#include "Uniforms.h"

#include <iostream>
#include <glad/glad.h>
#include <assert.h>


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
        uint32_t ID;
        UniformInfoBufferPtr UniformInfoBuffer;

        void create(const ShaderInstanceGL& vertexShader, const ShaderInstanceGL& fragmentShader);
        void findUniforms();
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

    void setProgram(ProgramHandle& handle) {
        assert(handle.isValid() && "::ERROR: Invalid program");
        glUseProgram(s_Programs[handle.idx].ID);
    }

    void destroy(ProgramHandle& handle) {
        s_Programs[handle.idx].destroy();
        HandleGenerator<ProgramHandle>::removeHandle(handle);
    }

}

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
            const char* nameStr = name.c_str();
            uint32_t location = glGetUniformLocation(ID, nameStr);

            UniformInfoBuffer->write(&type, sizeof(GLenum));
            UniformInfoBuffer->write(&location, sizeof(uint32_t));
        }
    }

    void ProgramInstanceGL::destroy() {
        glDeleteProgram(ID);
        ID = 0;
        UniformInfoBuffer = nullptr;
    }

}