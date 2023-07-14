#include "Renderer.h"

#include "shared/renderer/Context.h"
#include "shared/renderer/Programs.h"
#include "shared/renderer/VertexBuffer.h"
#include "shared/renderer/IndexBuffer.h"

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

            if(currentDrawCall.IndBufferHandle != renderItem.IndBufferHandle) {
                currentDrawCall.IndBufferHandle = renderItem.IndBufferHandle;
                spr::internal::bindIndexBuffer(renderItem.IndBufferHandle);
            }

            if(currentDrawCall.VertBufferHandle != renderItem.VertBufferHandle) {
                currentDrawCall.VertBufferHandle = renderItem.VertBufferHandle;
                changedAttributesLayout = true;

                spr::internal::bindVertexBuffer(renderItem.VertBufferHandle);
            }

            if(changedAttributesLayout) {
                const VertexBufferInstanceGL& vertexBuffer = spr::internal::getVertexBuffer(currentDrawCall.VertBufferHandle);
                spr::internal::bindVertexAttributeLayout(currentDrawCall.Program, vertexBuffer.LayoutHandle);
            }

            if(currentDrawCall.IndBufferHandle.isValid()) {
                // TODO: Draw elements (instanced by default? divisor = 0)
                const auto& indexBuffer = spr::internal::getIndexBuffer(currentDrawCall.IndBufferHandle);
                glDrawElements(GL_TRIANGLES, indexBuffer.IndexCount, GL_UNSIGNED_INT, NULL);
            }
            else {
                // TODO: Instanced by default? (w/ divisor = 0)
                const auto& vertexBuffer = spr::internal::getVertexBuffer(currentDrawCall.VertBufferHandle);
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
