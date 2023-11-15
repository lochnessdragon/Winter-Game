#include "application.h"

// Engine files
#include <log.h>
#include <glm/gtx/string_cast.hpp>
#include <scene/sprite.h>
#include <scene/transform.h>

Application::Application() {
	win = std::make_shared<Window>("Github Game Off 2023", 1200, 800);
	Log::getGameLog()->trace("Surface created");
	win->getKeyEventHandler().addListener([this](auto keyData) {
		if (keyData.key == GLFW_KEY_ESCAPE && keyData.action == GLFW_PRESS) {
			win->setShouldClose();
		}
	});

	// set up input system
	Input::setHandle(win->getHandle());

	renderer2d = std::make_shared<Renderer2D>();
	textRenderer = std::make_shared<TextRenderer>(win);

	// create a font
	font = std::make_shared<Font>("res/fonts/OpenSans-VariableFont_wdth,wght.ttf");

	glm::ivec2 winSize = win->getWindowSize();
	Log::getGameLog()->trace("Creating a camera: win_size x={} y={}", winSize.x, winSize.y);
	camera = std::make_shared<OrthoCamera>(win, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	renderer2d->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	deltaTime = 0.0f;
	lastTime = glfwGetTime();

	auto entity = scene.create();
	auto& transform = scene.emplace<Transform>(entity);
	scene.emplace<SpriteComponent>(entity);
	transform.scale *= 40;
	transform.position.x = 40;
	transform.position.y = 40;

	Log::getGameLog()->trace("Initialization finished");
}

void Application::update() {
	// tick
	{
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
	}

	// update dirty models
	auto transforms = scene.view<Transform>();
	transforms.each([](auto& transform) {
		if (transform.dirty) {
			transform.recalculateModelMat();
		}
	});

	// render
	renderer2d->startFrame();

	auto sprites = scene.view<Transform, SpriteComponent>();
	sprites.each([this](auto& transform, auto& sprite) {
		this->renderer2d->renderSprite(camera, transform, sprite.texture, sprite.color);
	});

	renderer2d->endFrame();

	textRenderer->render("Github Game Off - build alpha.1", font, { 5.0, 36.0 }, 36, { 1.0f, 1.0f, 1.0f, 1.0f });
	textRenderer->endFrame();
	win->swap();

	Input::update();
}