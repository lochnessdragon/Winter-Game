//#include <filesystem>
#include <iostream>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// engine files
#include <log.h>

// project files
#include "application.h"

/*
* Features to implements before Github Game Off 2023
* 1. 2D rendering
* 2. Text rendering
* 3. Scene system
* 4. Audio system
* 5. ImGUI
* 6. ECS
*/

Application* app;

void game_loop() {
	app->update();
}

int main() {
	try {
		Log::init();
		//Log::getGameLog()->info("CWD: {}", std::filesystem::current_path().string());
		Log::getGameLog()->info("Game startup...");
		app = new Application();

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(game_loop, 0, false);
#else
		while (!app->win->shouldClose()) {
			app->update();
		}
#endif

		return 0;
    } catch (std::exception& e) {
		std::cout << "FATAL ERROR: " << e.what() << std::endl;
	}

	return 1;
}
