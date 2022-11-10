#pragma once

#include <glm/glm.hpp>

#include "window.h"

class Camera {
private:
    glm::mat4 viewMat;
    glm::mat4 projMat;

    glm::vec3 position;
    glm::vec3 rotation;

    float aspect;
    float fov;
    float near;
    float far;

public:
    Camera(Window* window, glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far);
    ~Camera();

    void calculateViewMat();
    void calculateProjMat();

    void setPosition(glm::vec3 newPos) { position = newPos; calculateViewMat(); }

    glm::vec3 getPosition() { return position; }
    glm::vec3 getRotation() { return rotation; }

    glm::mat4 getViewMat() { return viewMat; }
    glm::mat4 getProjMat() { return projMat; }
};