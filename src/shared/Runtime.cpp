#include "Runtime.h"

#include "shared/layers/Layer.h"
#include "shared/layers/ExampleGUILayer.h"
#include "shared/layers/SandboxLayer.h"
#include "shared/ImGui/ImGuiLayer.h"

#include <spw/Event.h>
#include <spw/Inputs.h>
#include <spw/SimpleWindow.h>

Runtime *Runtime::get() {
	static Runtime s_Runtime;
	static bool s_Initialized = false;

	if (!s_Initialized) {
		s_Runtime.init();
		s_Initialized = true;
	}

	return &s_Runtime;
}

void Runtime::init() {
	initializeLayers();

	spw::registerEventListener([this](const Event &evt) {
		broadcastEvent(evt);
	});
}

void Runtime::attachLayer(Layer *layer) {
	m_Layers.push_back(layer);
	layer->OnAttach();
}

void Runtime::attachLayers(std::vector<Layer *> layers) {
	for (const auto &layer : layers) {
		attachLayer(layer);
	}
}

void Runtime::dettachLayer(Layer *layer) {
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end()) {
		Layer *dettached = *it;
		(dettached)->OnDetach();
		m_Layers.erase(it);

		delete dettached;
	}
}

void Runtime::dettachAllLayers() {
	while (m_Layers.size() > 0) {
		dettachLayer(m_Layers[0]);
	}
}

void Runtime::replaceLayers(std::vector<Layer *> layers) {
	m_NewLayers = layers;
}

void Runtime::initializeLayers() {
	m_ExampleLayer = new ExampleGUILayer();
	m_ExampleLayer->OnAttach();

	m_ImGuiLayer = new ImGuiLayer();
	m_ImGuiLayer->OnAttach();

	attachLayer(new SandboxLayer());
}

void Runtime::update() {
	static float lastFrame = 0.f;

	// Executes layers
	for (Layer *layer : m_Layers) {
		layer->OnUpdate();
	}

	// Executes ImGui layers
	if (m_ImGuiLayer) {
		m_ImGuiLayer->Begin();

		m_ExampleLayer->OnImGuiRender();
		for (int i = 0; i < m_Layers.size(); i++) {
			m_Layers[i]->OnImGuiRender();
		}

		m_ImGuiLayer->End();
	}

	// Calculates delta time
	const float currentFrame = getTime();
	m_DeltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Handles window close event
	if (spw::isKeyPressed(spw::KEY_ESCAPE) == GLFW_PRESS) {
		m_bIsActive = false;
		spw::requestCloseWindow();
	}

	// Quick hack to make resetting work
	if (m_NewLayers.size() > 0) {
		dettachAllLayers();
		attachLayers(m_NewLayers);
		m_NewLayers.clear();
	}
}

void Runtime::broadcastEvent(const Event &evt) {
	for (auto it = m_Layers.end(); it != m_Layers.begin();) {
		(*--it)->OnEvent(evt);
		if (evt.m_Handled)
			return;
	}
}

float Runtime::getTime() const {
	return spw::getTime();
}

