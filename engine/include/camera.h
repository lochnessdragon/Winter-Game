#pragma once

#include <glm/glm.hpp>

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
    Camera(glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far);
    ~Camera();

    void calculateViewMat();
    void calculateProjMat();

    glm::vec3 getPosition() { return position; }
    glm::vec3 getRotation() { return rotation; }

    glm::mat4 getViewMat() { return viewMat; }
    glm::mat4 getProjMat() { return projMat; }
};