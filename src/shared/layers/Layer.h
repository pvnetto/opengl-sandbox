#pragma once

#include <string>
#include <vector>

class Window;
class Event;

class Layer {
public:
	Layer(const std::string &name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Event& evt) {}

	inline const std::string &GetName() const { return m_name; }

private:
	std::string m_name;
};