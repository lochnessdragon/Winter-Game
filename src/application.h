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
#include <physics/world2d.h>
#include <ui/button.h>
#include "tilemap_editor.h"

// Game files
#include "draggable_component.h"
#include "dialogue.h"

class Application {
private:
	std::shared_ptr<PixelPerfectCamera> camera;
	std::shared_ptr<Renderer2D> renderer2d;
	std::shared_ptr<TextRenderer> textRenderer;

	std::shared_ptr<Font> font;
	std::shared_ptr<Font> large_font;
	std::shared_ptr<Font> small_font;

	std::shared_ptr<TilemapEditor> tilemapEditor;

	// textures
	std::shared_ptr<Texture> uiTexture;

	// Systems 
	std::shared_ptr<DialogueSystem> dialogue;
	//PhysicsWorld2D physics;

	entt::registry scene;
	entt::entity player;
	entt::entity bg;

	double deltaTime;
	double lastTime;

	bool showColliders;
public:
	std::shared_ptr<Window> win;

	Application();

	void update();
};