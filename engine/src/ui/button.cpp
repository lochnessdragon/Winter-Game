#include "ui/button.h"

#include <glm/gtx/string_cast.hpp>

#include "scene/transform.h"
#include "scene/colliders.h"
#include "scene/sprite.h"
#include "input.h"
#include "log.h"

ClickableSystem::ClickableSystem(std::shared_ptr<Window> win, entt::registry& scene) : win(win), scene(scene) {
	Input::get()->getMouseButtonEventHandler().addListener([this](auto& event) {
		if (event.action == GLFW_PRESS && event.button == GLFW_MOUSE_BUTTON_LEFT) {
			Log::getGameLog()->info("Left mouse button press event!");
			auto buttons = this->scene.view<Transform, RectangleCollider2D, Clickable>();

			// correct the mouse position
			glm::vec2 mousePos = Input::get()->getMousePos();
			glm::ivec2 winSize = this->win->getSize();
			mousePos.y = winSize.y - mousePos.y;

			Log::getGameLog()->trace("Mouse pos: {}", glm::to_string(mousePos));

			for (auto button : buttons) {
				auto [transform, collider] = buttons.get<Transform, RectangleCollider2D>(button);
				if (intersects(transform, collider, mousePos)) {
					auto& clickable = buttons.get<Clickable>(button);
					clickable.onClick();
					return true;
				}
			}
		}
		
		return false;
	});
}

entt::entity ClickableSystem::createButton(std::shared_ptr<Texture> spriteTex, std::function<ClickableFunc> clickCallback) {
	auto button = scene.create();

	scene.emplace<Transform>(button);
	auto& sprite = scene.emplace<SpriteComponent>(button);
	sprite.texture = spriteTex;
	scene.emplace<RectangleCollider2D>(button);

	// "clickable" component?
	auto& clickable = scene.emplace<Clickable>(button);
	clickable.onClick = clickCallback;

	return button;
}