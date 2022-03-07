#include <iostream>

#include "shared/renderer/SimpleRenderer.h"#include "shared/layers/SandboxLayer.h"


int main() {
	spr::createWindow("OpenGL Sandbox", 1280, 720);

	spr::init();

	while (spr::isWindowOpen()) {
		spr::clear();
		spr::runtime::update();
		spr::swapBuffers();
	}

	spr::shutdown();
	return 0;
}