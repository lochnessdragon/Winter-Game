#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

// todo: tie camera to window so that the camera will update its aspect ratio with the window.
Camera::Camera(Window* window, glm::vec3 position, glm::vec3 rotation) {
    this->position = position;
    this->rotation = rotation;

    calculateViewMat();
}

Camera::~Camera() {

}

void Camera::calculateViewMat() {
    this->viewMat = glm::identity<glm::mat4>();

    // glm::rotate(this->viewMat, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    // glm::rotate(this->viewMat, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    // glm::rotate(this->viewMat, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    this->viewMat = glm::translate(this->viewMat, -this->position);

    //std::cout << glm::to_string(this->position) << std::endl;
}

OrthoCamera::OrthoCamera(Window* window, glm::vec3 position, glm::vec3 rotation) : Camera(window, position, rotation) {
    this->calculateProjMat();
}

OrthoCamera::~OrthoCamera() {}

void OrthoCamera::calculateProjMat() {
    projMat = glm::ortho(0, 800, 0, 600);
}

PerspectiveCamera::PerspectiveCamera(Window* window, glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far) : aspect(aspect), fov(fov), near(near), far(far), Camera(window, position, rotation) {
    this->calculateProjMat();
    
    window->getWindowResizeHandler().addListener([=](const WindowResizeEventData& event) -> void {
        this->aspect = (float)event.width / (float)event.height;
        calculateProjMat();
    });

    window->getKeyPressedHandler().addListener([=](const KeyEventData& data) -> void {
        switch (data.key) {
        case GLFW_KEY_W:
            this->position.z -= 0.25f;
            this->calculateViewMat();
            break;
        case GLFW_KEY_A:
            this->position.x -= 0.25f;
            this->calculateViewMat();
            break;
        case GLFW_KEY_S:
            this->position.z += 0.25f;
            this->calculateViewMat();
            break;
        case GLFW_KEY_D:
            this->position.x += 0.25f;
            this->calculateViewMat();
            break;
        default:
            break;
        }
    });
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::calculateProjMat() {
    projMat = glm::perspective(this->fov, this->aspect, this->near, this->far);
}