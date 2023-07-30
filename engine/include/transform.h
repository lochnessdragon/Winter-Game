#pragma once

#include <glm/glm.hpp>

class Transform {
private:
    // quat
    glm::quat quaternionRotation;
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
