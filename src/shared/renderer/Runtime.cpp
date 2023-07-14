#include "Runtime.h"
#include "Context.h"

#include "shared/Event.h"
#include "shared/layers/Layer.h"
#include "shared/layers/ExampleGUILayer.h"
#include "shared/layers/SandboxLayer.h"
#include "shared/ImGui/ImGuiLayer.h"

#include <glad/glad.h>


namespace spr { namespace runtime {
    
    static std::vector<Layer*>  s_Layers;
    static std::vector<Layer*>  s_NewLayers;
    static ImGuiLayer*          s_ImGuiLayer    = nullptr;
    static ExampleGUILayer*     s_ExampleLayer  = nullptr;

    static bool s_InitializedRuntime = false;

    float s_DeltaTime = 0.f;

    void attachLayer(Layer* layer) {
        s_Layers.push_back(layer);
        layer->OnAttach();
    }

    static void attachLayers(std::vector<Layer*> layers) {
        for(const auto& layer : layers) {
            attachLayer(layer);
        }
    }

    void dettachLayer(Layer* layer) {
        auto it = std::find(s_Layers.begin(), s_Layers.end(), layer);
        if (it != s_Layers.end()) {
            Layer *dettached = *it;
            (dettached)->OnDettach();
            s_Layers.erase(it);

            delete dettached;
        }
    }

    static void dettachAllLayers() {
        while (s_Layers.size() > 0) {
            dettachLayer(s_Layers[0]);
        }
    }

    void replaceLayers(std::vector<Layer*> layers) {
        s_NewLayers = layers;
    }
    
    static void initializeLayers() {
        s_InitializedRuntime = true;
        
        s_ExampleLayer = new ExampleGUILayer();
        s_ExampleLayer->OnAttach();

        s_ImGuiLayer = new ImGuiLayer();
        s_ImGuiLayer->OnAttach();

        attachLayer(new SandboxLayer());
    }

    void update() {
        static float lastFrame = 0.f;

        if(!s_InitializedRuntime) {
            initializeLayers();
        }

        // Executes layers
        for (Layer *layer : s_Layers)
            layer->OnUpdate();

        // Executes ImGui layers
        if (s_ImGuiLayer) {
            s_ImGuiLayer->Begin();

            s_ExampleLayer->OnImGuiRender();
            for (int i = 0; i < s_Layers.size(); i++) {
                s_Layers[i]->OnImGuiRender();
            }
            
            s_ImGuiLayer->End();
        }

        // Calculates delta time
        float currentFrame = (float)glfwGetTime();
        s_DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handles window close event
        if (glfwGetKey(spr::getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(spr::getWindow(), true);
        }

        // Quick hack to make resetting work
        if(s_NewLayers.size() > 0) {
            dettachAllLayers();
            attachLayers(s_NewLayers);
            s_NewLayers.clear();
        }   
    }

    float getTime() {
        return (float) glfwGetTime();
    }

    float getDeltaTime() {
        return s_DeltaTime;
    }

}
}

namespace spr { namespace runtime {

    void handleEvent(Event &evt) {
        for (auto it = runtime::s_Layers.end(); it != runtime::s_Layers.begin();) {
            (*--it)->OnEvent(evt);
            if (evt.m_handled)
                return;
        }
    }

}}