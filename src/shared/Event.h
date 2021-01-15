#pragma once

#include <string>
#include <glm/glm.hpp>

class Event {
public:
	bool m_handled = false;

	virtual std::string GetName() = 0;
};

class MouseMovedEvent : public Event {
public:
	MouseMovedEvent(double x, double y, glm::vec2 mouseDelta) : m_mouseX(x), m_mouseY(y), m_mouseDelta(mouseDelta) {}

	inline double GetX() { return m_mouseX; }
	inline double GetY() { return m_mouseY; }

	inline const glm::vec2& MouseDelta() { return m_mouseDelta; }

	virtual std::string GetName() { return "MouseMoved"; }

private:
	double m_mouseX, m_mouseY;
	glm::vec2 m_mouseDelta;
};

class MouseButtonEvent : public Event {
public:
	inline int GetMouseButton() { return m_button; }
	virtual std::string GetName() = 0;

protected:
	MouseButtonEvent(int button) : m_button(button) {}
	int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
	MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
	virtual std::string GetName() { return "MouseButtonPressed"; }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
	MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
	virtual std::string GetName() { return "MouseButtonReleased"; }
};


class KeyPressedEvent : public Event {
public:
	KeyPressedEvent(int button) : m_button(button) { }
	virtual std::string GetName() { return "KeyPressedEvent"; }
	int GetKey() { return m_button; }

private:
	int m_button;

};