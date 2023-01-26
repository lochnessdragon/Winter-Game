#pragma once

#include <glm/glm.hpp>

#include "window.h"

class Camera {
protected:
    glm::mat4 viewMat;
    glm::mat4 projMat;

    glm::vec3 position;
    glm::vec3 rotation;

public:
    Camera(Window* window, glm::vec3 position, glm::vec3 rotation);
    ~Camera();

    virtual void calculateViewMat();
    virtual void calculateProjMat() = 0;

    void setPosition(glm::vec3 newPos) { position = newPos; calculateViewMat(); }

    glm::vec3 getPosition() { return position; }
    glm::vec3 getRotation() { return rotation; }

    glm::mat4 getViewMat() { return viewMat; }
    glm::mat4 getProjMat() { return projMat; }
};

class OrthoCamera : public Camera {
private:

public:
    OrthoCamera(Window* window, glm::vec3 position, glm::vec3 rotation);
    ~OrthoCamera();

    void calculateProjMat();
};

class PerspectiveCamera : public Camera {
private:
    float aspect;
    float fov;
    float near;
    float far;
public:
    PerspectiveCamera(Window* window, glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far);
    ~PerspectiveCamera();

    void calculateProjMat();
};