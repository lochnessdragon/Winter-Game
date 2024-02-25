#include "application.h"

// Engine files
#include <log.h>
#include <glm/gtx/string_cast.hpp>
#include <scene/sprite.h>
#include <scene/transform.h>
#include <scene/colliders.h>
#include <scene/rigidbodies.h>
#include <imgui_setup.h>
#include <misc/cpp/imgui_stdlib.h>
#include "player.h" 
#include "tilemap_editor.h"
#include "colors.h"
#include <audio/audio.h>

// implement 320 x 180 pixel-perfect camera
// 29 long dialogue line 1 (3, 75)
// 26 long dialoque line 2 (3, 84)

Application::Application() : showColliders(false) {
	win = std::make_shared<Window>("Cozy Winter Jam - Better Together", 960, 540);
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
	camera = std::make_shared<PixelPerfectCamera>(win, 160, 90, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	textRenderer = std::make_shared<TextRenderer>(camera->getFBO());

	// textures
	uiTexture = std::make_shared<Texture>("res/textures/ui.png");
	dialogue = std::make_shared<DialogueSystem>(uiTexture, "Howdy there, welcome to our little town. We share everything! That is to say...\n We\n want\n your\n stuff.\n :)");

	deltaTime = 0.0f;
	lastTime = glfwGetTime();

	// ecs systems
	player = scene.create();
	auto& transform = scene.emplace<Transform>(player);
	transform.position.x = 50;
	transform.position.y = 50;
	transform.scale *= 8;
	auto& sprite = scene.emplace<SpriteComponent>(player);
	sprite.texture = std::make_shared<Texture>("res/textures/player.png");
	auto& player_comp = scene.emplace<PlayerComponent>(player);
	auto& player_collider = scene.emplace<RectangleCollider2D>(player);
	player_collider.offset = glm::vec2(0, -5.0f);
	player_collider.size = glm::vec2(0.75f, 0.35f);
	// scene.emplace<Rigidbody2D>(player);
	player_comp.speed = 40.0f;
	player_comp.show_colliders = false;

	bg = scene.create();
	scene.emplace<Transform>(bg);
	scene.emplace<Tilemap>(bg, "res/rooms/home.bin", std::make_shared<Spriteset>(std::make_shared<Texture>("res/textures/tileset.png"), 8));
	auto& tilemapCollider = scene.emplace<TilemapCollider2D>(bg);
	tilemapCollider.impassable = { 0, 1, 2, 3, 4, 5, 6, 8, 10, 16, 18, 19, 20, 24, 25, 26, 60, 61, 62, 63 };

	tilemapEditor = std::make_shared<TilemapEditor>("res/rooms/home.bin", win);
	tilemapEditor->setTilemap(bg);
	
	// create a button
	/*{
		auto button = clickableSystem->createButton(std::make_shared<Texture>("res/textures/allow_button.png", TextureScaling::Nearest), []() { Log::getGameLog()->info("Button clicked!"); });
		auto& transform = scene.get<Transform>(button);
		transform.scale *= 160;
		transform.position.x = 200;
		transform.position.y = 200;
	}*/

	Log::getGameLog()->trace("Initialization finished");
}

void Application::update() {
	// tick
	{
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
	}

	// game logic
	if (!dialogue->enabled)
		player_move(deltaTime, scene, player, bg);
	tilemapEditor->update(scene);
	dialogue->update(deltaTime);

	// update dirty models
	auto transforms = scene.view<Transform>();
	transforms.each([](auto& transform) {
		if (transform.dirty) {
			transform.recalculateModelMat();
		}
	});

	// render
	camera->startScene();
	renderer2d->setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	renderer2d->startFrame(camera);
	auto& bgTransform = scene.get<Transform>(bg);
	auto& bgTilemap = scene.get<Tilemap>(bg);

	renderer2d->renderTilemap(bgTransform, bgTilemap);

	auto sprites = scene.view<Transform, SpriteComponent>();
	sprites.each([this](auto& transform, auto& sprite) {
		this->renderer2d->renderSprite(transform, sprite);
	});

	player_render(renderer2d, scene, player);

	dialogue->render(camera, renderer2d, textRenderer, large_font);

	renderer2d->endFrame();
	//textRenderer->render(dialogueInfo.text, large_font, { (float) dialogueInfo.x, (float) dialogueInfo.y }, 8, Colors::White);
	textRenderer->endFrame();
	camera->endScene();

	// dialogue text (A little funky, but like, who isn't?
	//textRenderer->render("Github Game Off - build alpha.1", large_font, { 5.0, 36.0 }, 36, Colors::White);
	//textRenderer->endFrame();

	imguiStartFrame();
	player_debug(scene, player);
	tilemapEditor->gui(scene);
	imguiEndFrame();

	win->swap();

	Input::get()->update();
}