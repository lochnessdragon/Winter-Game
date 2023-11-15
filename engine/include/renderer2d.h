#pragma once

#include <glm/glm.hpp>

/**
* We are reimplementing a lot of the 3d renderer methods. A better idea would be to have a base renderer class with derived rendering functionality.
* The base class handles overall stuff, such as clear color, clearing frame, opengl state management, whilst the renderers handle their specific subset of graphics features
* 
*/
class Renderer2D {
public:
	Renderer2D();

	void clear();
	void setClearColor(float red, float green, float blue, float alpha);
	void setClearColor(glm::vec4 color);

	void startFrame();

	void renderSprite(glm::vec4 color);
};