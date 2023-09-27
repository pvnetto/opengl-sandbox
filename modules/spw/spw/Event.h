#pragma once

#include <string>

namespace spw {

	struct Vec2i {
		int X, Y;

		Vec2i() = default;
		Vec2i(int inX, int inY) : X(inX), Y(inY) {}
	};

}

class Event {
public:
	mutable bool m_Handled = false;

	virtual std::string GetName() = 0;
};

class MouseMovedEvent : public Event {
public:
	MouseMovedEvent(double x, double y, spw::Vec2i mouseDelta) : m_MouseX(x), m_MouseY(y), m_MouseDelta(mouseDelta) {}

	inline double GetX() { return m_MouseX; }
	inline double GetY() { return m_MouseY; }

	inline const spw::Vec2i& MouseDelta() const { return m_MouseDelta; }

	virtual std::string GetName() { return "MouseMoved"; }

private:
	double m_MouseX, m_MouseY;
	spw::Vec2i m_MouseDelta;
};

class MouseButtonEvent : public Event {
public:
	inline int GetMouseButton() const { return m_Button; }
	virtual std::string GetName() = 0;

protected:
	MouseButtonEvent(int button) : m_Button(button) {}
	int m_Button;
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
	KeyPressedEvent(int button) : m_Button(button) { }
	virtual std::string GetName() { return "KeyPressedEvent"; }
	int GetKey() { return m_Button; }

private:
	int m_Button;

};