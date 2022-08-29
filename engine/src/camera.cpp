#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

// todo: tie camera to window so that the camera will update its aspect ratio with the window.
Camera::Camera(glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far) {
    this->position = position;
    this->rotation = rotation;
    this->fov = fov;
    this->near = near;
    this->far = far;
    this->aspect = aspect;

    calculateViewMat();
    calculateProjMat();
}

Camera::~Camera() {

}

void Camera::calculateViewMat() {
    this->viewMat = glm::identity<glm::mat4>();

    // glm::rotate(this->viewMat, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    // glm::rotate(this->viewMat, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    // glm::rotate(this->viewMat, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::translate(this->viewMat, -this->position);
}

void Camera::calculateProjMat() {
    projMat = glm::perspective(this->fov, this->aspect, this->near, this->far);
}