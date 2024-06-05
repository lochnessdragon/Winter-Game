#include "math/intersection.h"

bool intersects(glm::vec2 circlePos, float circleRadius, glm::vec2 pos)
{
	if (glm::length(circlePos - pos) < circleRadius)
		return true;
	return false;
}

/* intersects - returns true if the pos is within the vector defined by its center and size */
bool intersects(glm::vec2 rectPos, glm::vec2 rectSize, glm::vec2 pos)
{
	glm::vec2 halfSize = rectSize / 2.0f;
	if (pos.x > rectPos.x - halfSize.x && pos.x < rectPos.x + halfSize.x && pos.y > rectPos.y - halfSize.y && pos.y < rectPos.y + halfSize.y)
		return true;
	return false;
}
