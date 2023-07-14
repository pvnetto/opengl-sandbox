#pragma once

#include "shared/layers/Layer.h"
#include "shared/Shader.h"
#include "shared/Mesh.h"

class LOGL_05_Transform : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:
    Shader m_shader;
    Mesh m_cube;
    
};