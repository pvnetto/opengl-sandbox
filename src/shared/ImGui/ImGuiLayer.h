#pragma once

#include "shared/layers/Layer.h"

class Window;

/// <summary>
/// This layer contains boilerplate ImGui code so that other layers can use ImGui easily. It should
/// always be included in the stack of layers.
/// </summary>
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDettach() override;

    void Begin();
    void End();

};