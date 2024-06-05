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
#include "game.h"

Game* game = nullptr;

void game_loop() {
	game->update();
}

int main() {
	try {
		Log::init();
		//Log::getGameLog()->info("CWD: {}", std::filesystem::current_path().string());
		Log::getGameLog()->info("Game startup...");
		game = new Game();

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(game_loop, 0, false);
#else
		while (!game->win->shouldClose()) {
			game->update();
		}
#endif

		return 0;
    } catch (std::exception& e) {
		std::cout << "FATAL ERROR: " << e.what() << std::endl;
	}

	return 1;
}
