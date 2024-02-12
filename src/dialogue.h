#pragma once

#include <memory>
#include <vector>
#include <string>

#include <renderer2d.h>
#include <ui/text-renderer.h>

class DialogueSystem {
private:
	std::vector<std::string> lines;
	size_t currentLine;
	size_t currentChar;

	static const uint32_t Line1Max = 27;
	static const uint32_t Line2Max = 25;
	static const uint32_t LineX = 3;
	static const uint32_t Line1Y = 75;
	static const uint32_t Line2Y = 84;

	std::shared_ptr<Texture> uiTexture;

	Transform bgTransform;
	SpriteComponent bgSprite;

	Transform pointerTransform;
	SpriteComponent pointerSprite;

	double timer;
public:
	bool enabled;
	bool waitingOnUser;
	
	DialogueSystem(std::shared_ptr<Texture> uiTexture, std::string text);

	void setDialogue(std::string text);

	void update(double deltaTime);
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Renderer2D> renderer, std::shared_ptr<TextRenderer> textRenderer, std::shared_ptr<Font> largeFont);

};