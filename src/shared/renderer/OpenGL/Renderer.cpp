#include "shared/renderer/Context.h"
#include "shared/renderer/Programs.h"
#include "shared/renderer/VertexBuffer.h"

namespace spr {

    // ------------------======================================
    // ----- Implements spr::render from Context.h
    // ------------------======================================
    void render() {
        DrawCallData currentDrawCall;
        FrameData& frameData = spr::getFrameData();
        for(const DrawCallData& renderItem : frameData.DrawCalls) {
            spr::updateUniforms(frameData.UniformDataBuffer, renderItem.UniformsStart, renderItem.UniformsEnd);
            spr::rendererSetUniforms(getProgramUniforms(renderItem.Program));

            bool changedAttributesLayout = false;
            if(currentDrawCall.Program != renderItem.Program) {
                currentDrawCall.Program = renderItem.Program;
                changedAttributesLayout = true;

                spr::useProgram(renderItem.Program);
            }

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
                // TODO: Draw arrays (instanced by default? divisor = 0)
            }
        }
    }

}