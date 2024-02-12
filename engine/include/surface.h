#pragma once

#include <glm/glm.hpp>

/**
* Stores pixels, whether they be cpu bound, or gpu bound, their "bound" to be stored on this thing.
* Base class of Window and Framebuffer
*/
class Surface {
public:
	/**
	* getSize() - interface function
	* @returns the size of this surface
	*/
	virtual glm::ivec2 getSize() = 0;
};