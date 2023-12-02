#pragma once

#include "Event.h"

#include <functional>

namespace spw {

	using OnEventFn = std::function<void(const Event&)>;

	using GlLoaderResult = void(*)();
	using GlLoaderFn = GlLoaderResult(*)(const char*);

	void createWindow(const char *windowName, int width, int height);
	void destroyWindow();
	void requestCloseWindow();
	void swapBuffers();

	bool isWindowOpen();
	void* getWindowImpl();
	spw::Vec2 getWindowSize();
	int getWindowWidth();
	int getWindowHeight();
	float getWindowAspectRatio();

	GlLoaderFn getGlLoaderFn();

	void registerEventListener(OnEventFn eventListener);
	bool isKeyPressed(int key);
	void pollEvents();

	spw::Vec2 getMousePos();
	float getTime();

}
