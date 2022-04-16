#pragma once

#include "Uniforms.h"
#include "Handles.h"

namespace spr {

    ShaderHandle createShader(unsigned int shaderType, const char* shaderSrc);
    void destroy(ShaderHandle& shaderHandle);

    ProgramHandle createProgram(ShaderHandle& vertexHandle, ShaderHandle& fragmentHandle, bool destroyShaders = true);
    void destroy(ProgramHandle& programHandle);

    void useProgram(const ProgramHandle& handle);
    UniformInfoBufferPtr getProgramUniforms(const ProgramHandle& handle);
}


namespace spr { namespace internal {
    void bindVertexAttributeLayout(const ProgramHandle& programHandle, const VertexAttributeLayoutHandle& layoutHandle);
}}

enum ShaderType {
    SPR_VERTEX_SHADER = 0,
    SPR_FRAGMENT_SHADER,
};
