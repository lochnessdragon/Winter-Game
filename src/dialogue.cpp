#include "dialogue.h"

#include <glm/gtx/string_cast.hpp>

#include "colors.h"
#include <input.h>
#include <log.h>

DialogueSystem::DialogueSystem(std::shared_ptr<Texture> uiTexture, std::string text) : timer(0), uiTexture(uiTexture), currentLine(0), currentChar(0), enabled(false), waitingOnUser(false)
{
	setDialogue(text);

	background = SpriteActor(glm::vec2(160.0f / 2.0f, 26.0f / 2.0f), 0.0f, glm::vec2(160.0f / 2.0f, 26.0f / 2.0f), uiTexture, Colors::White, uiTexture->getUVCoordsFromPixels({ 0, 0, 160, 26 }));
	pointer = SpriteActor(glm::vec2(150.0f, 7.0f), 0.0f, glm::vec2(8.0f / 2.0f, 5.0f / 2.0f), uiTexture, Colors::White, uiTexture->getUVCoordsFromPixels({ 160, 0, 8, 5 }));

	Input::get()->getKeyEventHandler().addListener([this](const KeyEventData& event) {
		if (event.action == GLFW_PRESS && event.key == GLFW_KEY_Z) {
			if (waitingOnUser) {
				waitingOnUser = false;
				// check for last line
				if (currentLine + 1 == lines.size()) {
					enabled = false;
				}
				else {
					currentLine++;
					currentChar = 0;
				}
			}
			else {
				// skip dialogue
				waitingOnUser = true;
				timer = 0;
				if (currentLine % 2 == 0) {
					// may have to increase to a higher line count
					if (currentLine + 1 < lines.size())
						currentLine++;
				}
				// increase char pointer to end of line
				currentChar = lines[currentLine].size();
			}
			return true;
		}
		return false;
	});
}

void DialogueSystem::setDialogue(std::string text)
{
	lines.clear();
	currentLine = 0;
	currentChar = 0;
	timer = 0;
	// split dialogue up into lines wrapping words if necessary
	std::string substring = "";
	std::string word = "";
	size_t line = 0;
	for (size_t i = 0; i < text.length(); i++) {
		// check current line length
		int currentLineMax = line % 2 == 0 ? Line1Max : Line2Max;

		if (substring.length() > currentLineMax) {
			lines.push_back(substring);
			substring = "";
			line++;
		}

		char c = text[i];
		if (c == '\n') {
			// newline, flush string
			if (substring.length() > 0) {
				lines.push_back(substring);
				substring = "";
				line++;
			}
		}
		else if (c == ' ') {
			// space, create a new word.
			if (substring.size() + word.size() + 1 > currentLineMax) {
				// append substring, in the next substring, add word. 
				lines.push_back(substring);
				substring = word;
				word = "";
				line++;
			}
			else {
				if (substring.size() > 0)
					substring += " ";
				substring += word;
				word = "";
			}
		}
		else {
			word += c;
		}
	}

	if (substring.length() > 0) {
		lines.push_back(substring);
	}
}

void DialogueSystem::update(double deltaTime)
{
	if (enabled) {
		if (!waitingOnUser) {
			timer += deltaTime;
			if (timer > .1) {
				currentChar++;
				if (currentChar > lines[currentLine].size()) {
					if (currentLine % 2 == 1) {
						// odd, wait for user
						currentChar = lines[currentLine].size();
						waitingOnUser = true;
					}
					else if (currentLine + 1 == lines.size()) {
						// last line, wait for user
						currentChar = lines[currentLine].size();
						waitingOnUser = true;
					}
					else {
						// next line
						currentLine++;
						currentChar = 0;
					}
				}
				timer = 0;
			}
		}
	}
}

void DialogueSystem::render(std::shared_ptr<Renderer2D> renderer, std::shared_ptr<TextRenderer> textRenderer, std::shared_ptr<Font> largeFont)
{
	if (enabled) {
		background.render(renderer);

		size_t line1 = currentLine % 2 == 0 ? currentLine : currentLine - 1;
		if (line1 == currentLine) {
			textRenderer->render(lines[line1].substr(0, currentChar), largeFont, { LineX, Line1Y }, 8, Colors::White);
		}
		else {
			textRenderer->render(lines[line1], largeFont, { LineX, Line1Y }, 8, Colors::White);
			textRenderer->render(lines[currentLine].substr(0, currentChar), largeFont, { LineX, Line2Y }, 8, Colors::White);
		}

		if (waitingOnUser) {
			pointer.render(renderer);
		}
		
	}
}
