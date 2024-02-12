#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
private:
    // quat
    // glm::quat quaternionRotation;

    glm::mat4 modelMat = glm::identity<glm::mat4>();
public:
    glm::vec3 position = { 0, 0, 0 };
    glm::vec3 rotation = { 0, 0, 0 };
    glm::vec3 scale = { 1, 1, 1 };
    bool dirty = true;

    void recalculateModelMat() {
        glm::mat4 rotationMat = glm::toMat4(glm::quat(this->rotation));
        modelMat = glm::translate(glm::mat4(1.0f), position)
            * rotationMat
            * glm::scale(glm::mat4(1.0f), scale);
        dirty = false;
    };

    glm::mat4 getModelMat() { return modelMat; };
};
