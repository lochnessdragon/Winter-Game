#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

class PhysicsWorld2D {
private:
	// glm::vec2 gravity = { 0.0f, -9.8f };
	void kinematics(float deltaTime, entt::registry& scene);
	void detect(float deltaTime, entt::registry& scene);
	void response(float deltaTime, entt::registry& scene);
public:
	void step(float deltaTime, entt::registry& scene);
};