#pragma once

// System files
#include <memory>

// Engine - dependency files
#include <entt/entt.hpp>

// Engine files
#include <window.h>
#include <camera.h>
#include <renderer2d.h>
#include <input.h>
#include <ui/text-renderer.h>
#include <scene/transform.h>

class Application {
private:
	std::shared_ptr<OrthoCamera> camera;
	std::shared_ptr<Renderer2D> renderer2d;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<Font> font;

	// textures

	entt::registry scene;

	double deltaTime;
	double lastTime;
public:
	std::shared_ptr<Window> win;

	Application();

	void update();
};