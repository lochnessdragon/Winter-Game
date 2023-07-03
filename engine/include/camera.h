#pragma once

#include <glm/glm.hpp>

#include "window.h"
#include <memory>

class Camera {
protected:
    glm::mat4 viewMat;
    glm::mat4 projMat;

    glm::vec3 position;
    glm::vec3 rotation;

public:
    Camera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation);
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
    glm::ivec2 fbSize;
public:
    OrthoCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation, glm::ivec2 size);
    ~OrthoCamera();

    void calculateProjMat();
};

class PerspectiveCamera : public Camera {
private:
    float aspect;
    float fov; // in radians (don't forget glm::radians)
    float near;
    float far;
public:
    PerspectiveCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far);
    ~PerspectiveCamera();

    void calculateProjMat();
};