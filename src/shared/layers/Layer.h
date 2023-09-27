#pragma once

#include <string>
#include <vector>

class Event;

class Layer {
public:
	Layer(const std::string &name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(const Event& evt) {}

	inline const std::string& GetName() const { return m_Name; }

private:
	std::string m_Name = "Default";

};