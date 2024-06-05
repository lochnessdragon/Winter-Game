#pragma once

#include <glm/glm.hpp>

#include "math/intersection.h"
#include "transform.h"

#include <vector>

// Consider Polymorphism
//enum class Collider2DType {
//	CIRCLE,
//	RECTANGLE,
//	TILEMAP,
//};
//
//struct Collider2D {
//	Collider2DType type;
//};

struct CircleCollider2D {
	glm::vec2 offset = { 0.0f, 0.0f };
	float radius = 1.0f;
};

struct RectangleCollider2D {
	glm::vec2 offset = { 0.0f, 0.0f };
	glm::vec2 size = { 1.0f, 1.0f };
};

struct TilemapCollider2D {
	// must be matched with a tilemap
	std::vector<int> impassable;
};

bool intersects(Transform& transform, CircleCollider2D& collider, glm::vec2 pos);
bool intersects(Transform& transform, RectangleCollider2D& collider, glm::vec2 pos);