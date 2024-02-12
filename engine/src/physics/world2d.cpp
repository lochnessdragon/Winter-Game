#include "physics/world2d.h"

#include <scene/transform.h>
#include <scene/rigidbodies.h>
#include <scene/colliders.h>
#include <tilemap.h>

void PhysicsWorld2D::step(float deltaTime, entt::registry& scene) {
	// kinematics
	auto& physical_objs = scene.view<Transform, Rigidbody2D>();
	auto& tilemaps = scene.view<Transform, TilemapCollider2D, Tilemap>();

	physical_objs.each([this, deltaTime](auto& transform, auto& rb) {
		if (rb.enabled) {
			// rb.acceleration += this->gravity;
			rb.velocity += rb.acceleration * deltaTime;
			transform.position += glm::vec3(rb.velocity, 0.0f) * deltaTime;
			transform.dirty = true;

			rb.acceleration = { 0, 0 };
		}
	});

	// detection

	// response
}