#pragma once

#include "shared/layers/Layer.h"

class LOGL_02_EBO : public Layer {
public:
    virtual void OnAttach() override;
	virtual void OnUpdate() override;

private:
    unsigned int vao, vbo, ebo;
    unsigned int m_shaderProgram;
};

