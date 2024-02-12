#pragma once

#include <entt/entt.hpp>

#include "window.h"
#include "texture.h"

typedef void ClickableFunc(void);

struct Clickable {
	std::function<ClickableFunc> onClick;
};

class ClickableSystem {
private:
	std::shared_ptr<Window> win;
	entt::registry& scene;
public:
	ClickableSystem(std::shared_ptr<Window> win, entt::registry& scene);

	// there are two types of buttons 
	// a sprite button, with only a bg texture
	// a regular button, with a bg texture and a text component

	entt::entity createButton(std::shared_ptr<Texture> spriteTex, std::function<ClickableFunc> clickCallback);
};