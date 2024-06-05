#pragma once

#include <glm/glm.hpp>

bool intersects(glm::vec2 circlePos, float circleRadius, glm::vec2 pos);
bool intersects(glm::vec2 rectPos, glm::vec2 rectSize, glm::vec2 pos);