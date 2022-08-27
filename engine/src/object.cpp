#include <object.h>

Object::Object(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, Mesh& mesh) : position(position), scale(scale), rotation(rotation), mesh(mesh) {
    calculateModelMat();
    this->dirty = false;
}

Object::~Object() {}

void Object::calculateModelMat() {
    this->modelMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat, position);
    modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::scale(modelMat, scale);
}