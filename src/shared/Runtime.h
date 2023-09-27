#pragma once

#include <vector>

class Layer;
class Event;
class ImGuiLayer;
class ExampleGUILayer;

class Runtime {
private:
	Runtime() = default;

public:
	static Runtime *get();

	void init();

public:
	void attachLayer(Layer *layer);
	void attachLayers(std::vector<Layer *> layers);
	void dettachLayer(Layer *layer);
	void replaceLayers(std::vector<Layer *> layers);

private:
	void initializeLayers();
	void dettachAllLayers();

public:
	void update();

	float getTime() const;
	inline float getDeltaTime() const { return m_DeltaTime; }

private:
	void broadcastEvent(const Event &evt);

private:
	std::vector<Layer *> m_Layers;
	std::vector<Layer *> m_NewLayers;
	ImGuiLayer *m_ImGuiLayer = nullptr;
	ExampleGUILayer *m_ExampleLayer = nullptr;

private:
	float m_DeltaTime = 0.f;
};
