#pragma once

#include "Uniforms.h"
#include "Handles.h"

namespace spr {

    ShaderHandle createShader(unsigned int shaderType, const char* shaderSrc);
    void destroy(ShaderHandle& shaderHandle);

    ProgramHandle createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders = true);
    void destroy(ProgramHandle& programHandle);

    void setProgram(const ProgramHandle& handle);
    UniformInfoBufferPtr getProgramUniforms(const ProgramHandle& handle);
}


enum ShaderType {
    SPR_VERTEX_SHADER = 0,
    SPR_FRAGMENT_SHADER,
};
