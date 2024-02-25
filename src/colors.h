#pragma once

#include <glm/glm.hpp>

namespace Colors {
	static const glm::vec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const glm::vec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const glm::vec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	static const glm::vec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	static const glm::vec4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };

	//glm::vec4 fromRGBA(int r, int g, int b, int a) { return { (float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f }; }
}