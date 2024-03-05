#pragma once

// System files
#include <memory>

// Engine files
#include <window.h>
#include <camera.h>
#include <renderer2d.h>
#include <input.h>
#include <ui/text-renderer.h>
#include <scene/transform.h>
#include "tilemap_editor.h"
#include "player.h"
#include "room_manager.h"

// Game files
#include "dialogue.h"

class Game {
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
	RoomManager roomManager;

	// Actors
	std::shared_ptr<Tilemap> bg;
	std::shared_ptr<Player> player;

	double deltaTime;
	double lastTime;

	bool showColliders;

	void setupRoomManager();
public:
	std::shared_ptr<Window> win;

	Game();

	void update();
};