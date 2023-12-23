#include <iostream>

#include "shared/layers/SandboxLayer.h"
#include "shared/Runtime.h"

#include <spr/SimpleRenderer.h>
#include <spw/SimpleWindow.h>

int main() {

	// Initialization
	{
		spw::createWindow("OpenGL Sandbox", 1920, 1080);
		
		spr::ContextInfo info;
		info.Width = spw::getWindowWidth();
		info.Height = spw::getWindowHeight();
		info.GlLoader = spw::getGlLoaderFn();
		spr::init(info);
	}

	// Game Loop
	{
		while (spw::isWindowOpen()) {
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