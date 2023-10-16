#pragma once

#include "shared/layers/Layer.h"

#include <unordered_map>
#include <memory>

class Example {
public:
    std::string m_name;
    bool m_selected = false;

    Example(std::string name) : m_name(name) { }
    virtual std::vector<Layer*> GetLayers() = 0;
};

template <typename T>
class SimpleExample : public Example {
public:
    SimpleExample(std::string name) : Example(name) { }

    virtual std::vector<Layer*> GetLayers() {
        return std::vector<Layer*>{new T()};
    }
};

enum ExampleCategory { LearnOpenGL, AdvancedOpenGL, RendererExamples, CategoryCount };

class ExampleGUILayer : public Layer {
public:
    ExampleGUILayer();
    virtual void OnImGuiRender() override;

private:
    int m_category = 0;
    int m_example = 0;

    std::unordered_map<ExampleCategory, std::vector<std::shared_ptr<Example>>> m_examples;
    std::shared_ptr<Example> m_selected = nullptr;
};