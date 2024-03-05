#include "game.h"

// Engine files
#include <log.h>
#include <glm/gtx/string_cast.hpp>
#include <scene/transform.h>
#include <scene/colliders.h>
#include <scene/rigidbodies.h>
#include <imgui_setup.h>
#include <misc/cpp/imgui_stdlib.h>
#include "player.h" 
#include "tilemap_editor.h"
#include "colors.h"
#include <audio/audio.h>
#include "consts.h"

// implement 320 x 180 pixel-perfect camera
// 29 long dialogue line 1 (3, 75)
// 26 long dialoque line 2 (3, 84)

// TODO:
// - proper layering/z-sorting
// - 2d lighting
// - proper collision detection
// - actor collision
// - custom objects in tilemap files

Game::Game() : showColliders(false) {
	win = std::make_shared<Window>("Better Together", 960, 540);
	Log::getGameLog()->trace("Surface created");
	Input::get()->getKeyEventHandler().addListener([this](auto keyData) {
		if (keyData.key == GLFW_KEY_ESCAPE && keyData.action == GLFW_PRESS) {
			win->setShouldClose();
			return true;
		}
		return false;
	});
	Input::get()->joystickInfo();
	Input::get()->map("res/input.json");

	// renderers
	renderer2d = std::make_shared<Renderer2D>();
	renderer2d->setLineWidth(2.0f);

	// setup imgui
	setupImgui(win);

	// create a font
	font = std::make_shared<Font>("res/fonts/OpenSans-VariableFont_wdth,wght.ttf");
	small_font = std::make_shared<Font>("res/fonts/objectives.ttf");
	large_font = std::make_shared<Font>("res/fonts/slkscr.ttf");

	glm::ivec2 winSize = win->getSize();
	Log::getGameLog()->trace("Creating a camera: win_size x={} y={}", winSize.x, winSize.y);
	camera = std::make_shared<PixelPerfectCamera>(win, SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	textRenderer = std::make_shared<TextRenderer>(camera->getFBO());

	// textures
	uiTexture = std::make_shared<Texture>("res/textures/ui.png");
	dialogue = std::make_shared<DialogueSystem>(uiTexture, "Howdy there, welcome to our little town. We share everything! That is to say...\n We\n want\n your\n stuff.\n :)");

	deltaTime = 0.0f;
	lastTime = glfwGetTime();

	// actors
	player = std::make_shared<Player>(40.0f);

	// this does 2 reads, we should optimize
	bg = std::make_shared<Tilemap>(0, 0, std::make_shared<Spriteset>(std::make_shared<Texture>("res/textures/tileset.png"), 8));
	bg->impassable = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 16, 18, 19, 20, 24, 25, 26, 60, 61, 62, 63 };
	
	setupRoomManager();
	roomManager.setTilemap(bg);
	player->position = roomManager.enter("home");

	tilemapEditor = std::make_shared<TilemapEditor>("res/rooms/home.bin", win, bg);

	Log::getGameLog()->trace("Initialization finished");
}

void Game::update() {
	// tick
	{
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
	}

	// game logic
	if (!dialogue->enabled)
		player->update(deltaTime, bg);
	tilemapEditor->update();
	dialogue->update(deltaTime);

	// render
	camera->startScene();
	renderer2d->setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	renderer2d->startFrame(camera);
	
	bg->render(renderer2d);

	player->render(renderer2d);

	dialogue->render(renderer2d, textRenderer, large_font);

	renderer2d->endFrame();
	//textRenderer->render(dialogueInfo.text, large_font, { (float) dialogueInfo.x, (float) dialogueInfo.y }, 8, Colors::White);
	textRenderer->endFrame();
	camera->endScene();

	// dialogue text (A little funky, but like, who isn't?
	//textRenderer->render("Github Game Off - build alpha.1", large_font, { 5.0, 36.0 }, 36, Colors::White);
	//textRenderer->endFrame();

	imguiStartFrame();
	player->imgui();
	tilemapEditor->imgui();
	imguiEndFrame();

	win->swap();

	Input::get()->update();
}

void Game::setupRoomManager() {
	RoomEntry home;
	home.filename = "res/rooms/home.bin";
	home.start_pos = { 50.0f, 50.0f };
	home.down = "outside_home";
	roomManager.configRoom("home", home);

	RoomEntry outsideHome;
	outsideHome.filename = "res/rooms/outside1.bin";
	roomManager.configRoom("outside_home", outsideHome);
}