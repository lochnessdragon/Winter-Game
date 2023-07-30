#pragma once

#include <glm/glm.hpp>

struct Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct AmbientLight {
public:
    glm::vec3 color;
    float intensity;
};

struct SpotLight : Light {
public:
	glm::vec3 direction;
	float cutoffAngle;
};
