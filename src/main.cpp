#include <iostream>

#include "shared/layers/SandboxLayer.h"
#include "shared/Runtime.h"

#include <spr/SimpleRenderer.h>
#include <spw/SimpleWindow.h>

int main() {

	// Initialization
	{
		spw::createWindow("OpenGL Sandbox", 1280, 720);
		
		spr::ContextInfo info;
		info.Width = spw::getWindowWidth();
		info.Height = spw::getWindowHeight();
		info.GlLoader = spw::getGlLoaderFn();
		spr::init(info);
	}

	// Game Loop
	{
		while (spw::isWindowOpen()) {
			spr::clear();

			Runtime::get()->update();
			
			spw::swapBuffers();
			spw::pollEvents();
		}
	}

	// Shutdown
	{
		spr::shutdown();
		spw::destroyWindow();
	}
	return 0;
}