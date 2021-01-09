#pragma once

#include <string>

class Layer {
public:
	Layer(const std::string &name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDettach() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}

	inline const std::string &GetName() const { return m_name; }

private:
	std::string m_name;
};