#include "Programs.h"
#include "Uniforms.h"

#include <iostream>
#include <glad/glad.h>


// ========================================
// ==== Shaders ===========================
// ========================================

namespace spr {

    struct ShaderInstance {
        uint32_t ID;
    };

    struct ProgramInstance {
        uint32_t ID;
        UniformInfoBufferPtr UniformInfoBuffer;
    };

    static ShaderInstance s_Shaders[ShaderHandle::capacity];
    static ProgramInstance s_Programs[ProgramHandle::capacity];

}

namespace spr {

    static GLuint getGLShaderTypeFromSPR(unsigned int sprShaderType) {
        if(sprShaderType == SPR_VERTEX_SHADER) return GL_VERTEX_SHADER;
        if(sprShaderType == SPR_FRAGMENT_SHADER) return GL_FRAGMENT_SHADER;

        return GL_VERTEX_SHADER;
    }
    
    ShaderHandle createShader(unsigned int shaderType, const char* shaderSrc) {
        unsigned int glShaderType = getGLShaderTypeFromSPR(shaderType);

        ShaderHandle shaderHandle;
        shaderHandle.idx = glCreateShader(glShaderType);
        glShaderSource(shaderHandle.idx, 1, &shaderSrc, NULL);
        glCompileShader(shaderHandle.idx);

        int success;
        glGetShaderiv(shaderHandle.idx, GL_COMPILE_STATUS, &success);
        if(!success) {
            char info[512];
            glGetShaderInfoLog(shaderHandle.idx, 512, NULL, info);
            std::cout << "::ERROR: Failed to compile shader:: " << info << "\n";
        }

        return shaderHandle;
    }

    void destroy(ShaderHandle& shaderHandle) {
        glDeleteShader(shaderHandle.idx);
        shaderHandle.idx = 0;
    }
}


// ========================================
// ==== Programs ==========================
// ========================================
namespace spr {

    ProgramHandle createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders) {
        // TODO: Generate handle
        // TODO: Initialize ProgramRef[handle]

        // TODO: Move this to inside
        ProgramHandle programHandle;
        programHandle.idx = glCreateProgram();
        glAttachShader(programHandle.idx, vertexHandle.idx);
        glAttachShader(programHandle.idx, fragmentHandle.idx);
        glLinkProgram(programHandle.idx);

        int linkSuccess;
        glGetProgramiv(programHandle.idx, GL_LINK_STATUS, &linkSuccess);
        if(!linkSuccess) {
            char linkInfo[512];
            glGetProgramInfoLog(programHandle.idx, 512, NULL, linkInfo);
            std::cout << "::ERROR: Failed to link shader " << linkInfo << "\n";
        }

        if(destroyShaders) {
            destroy(vertexHandle);
            destroy(fragmentHandle);
        }

        // TODO: Initialize ProgramInstance[handle]

        return programHandle;
    }

    void setProgram(ProgramHandle& handle) {
        glUseProgram(handle.idx);
    }

    void destroy(ProgramHandle& programHandle) {
        glDeleteProgram(programHandle.idx);
    }

}