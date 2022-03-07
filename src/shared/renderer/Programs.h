#pragma once

#include "Handles.h"

namespace spr {

    ShaderHandle createShader(unsigned int shaderType, const char* shaderSrc);
    void destroy(ShaderHandle& shaderHandle);

    ProgramHandle createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders = true);
    void setProgram(ProgramHandle& handle);
    void destroy(ProgramHandle& programHandle);

}


enum ShaderType {
    SPR_VERTEX_SHADER = 0,
    SPR_FRAGMENT_SHADER,
};
