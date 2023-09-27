#pragma once

#include "Event.h"

#include <functional>

namespace spw {

	using OnEventFn = std::function<void(const Event&)>;

	void createWindow(const char *windowName, int width, int height);
	void destroyWindow();
	void requestCloseWindow();
	void swapBuffers();

	void* getWindowImpl();
	spw::Vec2i getWindowSize();
	int getWindowWidth();
	int getWindowHeight();

	void registerEventListener(OnEventFn eventListener);
	bool isKeyPressed(int key);
	void pollEvents();

	spw::Vec2i getMousePos();
	float getTime();

}
