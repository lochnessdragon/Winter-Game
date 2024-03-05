#include "scene/actor.h"

Actor::Actor(glm::vec2 position, float rotation, glm::vec2 scale) : position(position), rotation(rotation), scale(scale) {
	recalculateModelMat();
}