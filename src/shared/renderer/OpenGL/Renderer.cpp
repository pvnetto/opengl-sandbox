#include "Renderer.h"

#include "shared/renderer/Context.h"
#include "shared/renderer/Programs.h"
#include "shared/renderer/VertexBuffer.h"

#include <glad/glad.h>

namespace spr {

    static uint32_t s_DefaultVAO = 0;

    void rendererInit() {
        glCreateVertexArrays(1, &s_DefaultVAO);
    }

    // ------------------======================================
    // ----- Implements some functions from Context.h
    // ------------------======================================
    void render() {
        DrawCallData currentDrawCall;

        glBindVertexArray(s_DefaultVAO);

        FrameData& frameData = spr::getFrameData();
        for(const DrawCallData& renderItem : frameData.DrawCalls) {
            spr::updateUniforms(frameData.UniformDataBuffer, renderItem.UniformsStart, renderItem.UniformsEnd);

            bool changedAttributesLayout = false;
            if(currentDrawCall.Program != renderItem.Program) {
                currentDrawCall.Program = renderItem.Program;
                changedAttributesLayout = true;

                spr::useProgram(renderItem.Program);
            }

            spr::rendererSetUniforms(getProgramUniforms(renderItem.Program));

            if(currentDrawCall.VertexBufferHandle != renderItem.VertexBufferHandle) {
                currentDrawCall.VertexBufferHandle = renderItem.VertexBufferHandle;
                changedAttributesLayout = true;

                spr::internal::bindVertexBuffer(renderItem.VertexBufferHandle);
            }

            if(currentDrawCall.IndexBufferHandle != renderItem.IndexBufferHandle) {
                currentDrawCall.IndexBufferHandle = renderItem.IndexBufferHandle;
                // TODO: Bind index buffer
            }

            if(changedAttributesLayout) {
                const VertexBufferInstanceGL& vertexBuffer = spr::internal::getVertexBuffer(currentDrawCall.VertexBufferHandle);
                spr::internal::bindVertexAttributeLayout(currentDrawCall.Program, vertexBuffer.LayoutHandle);
            }

            if(currentDrawCall.IndexBufferHandle.isValid()) {
                // TODO: Draw elements (instanced by default? divisor = 0)
            }
            else {
                // TODO: Instanced by default? (w/ divisor = 0)
                const auto& vertexBuffer = spr::internal::getVertexBuffer(currentDrawCall.VertexBufferHandle);
                const auto& vertexAttributeLayout = spr::getVertexAttributeLayout(vertexBuffer.LayoutHandle);
                const int vertexCount = vertexBuffer.Size / vertexAttributeLayout.getStride();
                glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            }
        }
    }

    void rendererShutdown() {
        glDeleteVertexArrays(1, &s_DefaultVAO);
    }

}