#include "scene/colliders.h"

/**
* intersects - calculates whether a position is intersecting a circle, given with a CircleCollider2D and adjusted by a Transform
*/
bool intersects(Transform& transform, CircleCollider2D& collider, glm::vec2 pos) {
	return intersects(glm::vec2(transform.position) + collider.offset, collider.radius * transform.scale.x, pos);
}

/**
* intersects - calculates whether a position is intersecting a rectangle, given with a RectangleCollider2D and adjusted by a Transform
*/
bool intersects(Transform& transform, RectangleCollider2D& collider, glm::vec2 pos) {
	return intersects(glm::vec2(transform.position) + collider.offset, glm::vec2(collider.size.x * transform.scale.x * 2.0f, collider.size.y * transform.scale.y * 2.0f), pos);
}