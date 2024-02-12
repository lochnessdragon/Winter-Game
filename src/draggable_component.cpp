#include "draggable_component.h"

#include <input.h>

DraggableSystem::DraggableSystem(std::shared_ptr<Window> win, entt::registry& scene) : win(win), scene(scene), prevMousePos(Input::get()->getMousePos()), currEntity() {
	Input::get()->getMouseButtonEventHandler().addListener([this](auto& event) {
		if (event.button == GLFW_MOUSE_BUTTON_LEFT) {
			if (event.action == GLFW_PRESS) {
				// find entity
				auto draggables = this->scene.view<Transform, CircleCollider2D, DraggableComponent>();
				glm::vec2 mousePos = Input::get()->getMousePos();
				glm::ivec2 winSize = this->win->getSize();
				mousePos.y = winSize.y - mousePos.y;

				for (auto entity : draggables) {
					auto [transform, collider] = draggables.get<Transform, CircleCollider2D>(entity);
					if (intersects(transform, collider, mousePos)) {
						currEntity = entity;
						dragging = true;
						prevMousePos = mousePos;
						return true;
					}
				}
			}
			else {
				// stop dragging
				dragging = false;
			}
		}
		return false;
	});
}

void DraggableSystem::loop() {
	if (dragging) {
		auto& transform = scene.get<Transform>(currEntity);
		glm::vec2 mousePos = Input::get()->getMousePos();
		glm::ivec2 winSize = this->win->getSize();
		mousePos.y = winSize.y - mousePos.y;
		glm::vec2 deltaMousePos = mousePos - prevMousePos;
		transform.position.x += deltaMousePos.x;
		transform.position.y += deltaMousePos.y;
		transform.dirty = true;
		prevMousePos = mousePos;
	}
}