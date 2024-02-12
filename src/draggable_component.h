#pragma once

// system files
#include <memory>

// Engine - dependency files
#include <entt/entt.hpp>

// Engine files
#include <window.h>
#include "scene/transform.h"
#include <scene/colliders.h>

struct DraggableComponent {};

class DraggableSystem {
private:
	bool dragging = false;

	std::shared_ptr<Window> win;
	entt::registry& scene;
	entt::entity currEntity;

	glm::vec2 prevMousePos;
public:
	DraggableSystem(std::shared_ptr<Window> win, entt::registry& scene);

	void loop();
};