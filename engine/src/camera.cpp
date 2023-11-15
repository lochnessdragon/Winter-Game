#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <log.h>

#include <iostream>

// todo: tie camera to window so that the camera will update its aspect ratio with the window.
Camera::Camera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation) {
	this->m_position = position;
    this->m_eulerRotation = rotation;
	this->rotation = glm::quat(rotation);

    calculateViewMat();
}

Camera::~Camera() {

}

void Camera::calculateViewMat() {
    this->viewMat = glm::identity<glm::mat4>();
	
	glm::quat rotWorldY = glm::angleAxis(this->m_eulerRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotWorldX = glm::angleAxis(this->m_eulerRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat rotWorldZ = glm::angleAxis(this->m_eulerRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));;

	this->rotation = rotWorldX * rotWorldY * rotWorldZ; // glm::quat(this->m_eulerRotation);
	viewMat *= glm::toMat4(rotation);
	
    this->viewMat = glm::translate(this->viewMat, -this->m_position);
}

glm::mat4 Camera::getViewMat() {
	if(dirty) {
		calculateViewMat();
		dirty = false;
	}
	return viewMat;
}

OrthoCamera::OrthoCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation) : Camera(window, position, rotation), fbSize(window->getWindowSize()) {
    this->calculateProjMat();

    window->getWindowResizeHandler().addListener([=](const WindowResizeEventData& event) -> void {
        this->fbSize = glm::ivec2(event.width, event.height);
    });
}

OrthoCamera::~OrthoCamera() {}

void OrthoCamera::calculateProjMat() {
    projMat = glm::ortho(0.0f, (float) this->fbSize.x, 0.0f, (float) this->fbSize.y, -1.0f, 1.0f);
    //Log::getRendererLog()->trace("Projection Mat: {}\n Fb: {}", glm::to_string(projMat), glm::to_string(this->fbSize));
}

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far) : Camera(window, position, rotation), aspect(aspect), fov(fov), near(near), far(far) {
    this->calculateProjMat();
    
    window->getWindowResizeHandler().addListener([=](const WindowResizeEventData& event) -> void {
        this->aspect = (float)event.width / (float)event.height;
        calculateProjMat();
    });
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::calculateProjMat() {
    projMat = glm::perspective(this->fov, this->aspect, this->near, this->far);
}
