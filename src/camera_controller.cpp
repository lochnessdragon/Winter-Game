#include "camera_controller.h"

#include <input.h>
#include <log.h>
#include <algorithm>

CameraController::CameraController() : moveSpeed(5.0f), rotSpeed(1.5f), prevMousePos(0.0f) {}

CameraController::CameraController(std::shared_ptr<Camera> cam) : camera(cam), moveSpeed(5.0f), rotSpeed(1.5f), prevMousePos(0.0f) {}

void CameraController::update(double dTime) {
	glm::vec3& position = this->camera->position();
	glm::vec3& rotation = this->camera->eulerRotation();
	// forward with orientation 0 should be in the x direction,
	// so, on this unit circle z = x; x = y
	glm::vec3 forward = glm::vec3(-sinf(rotation.y), 0.0f, cosf(rotation.y));
	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	
	if (Input::get()->isKeyPressed(GLFW_KEY_D)) {
		position -= right * (float) (dTime * moveSpeed);
		this->camera->setDirty();
	}
	if (Input::get()->isKeyPressed(GLFW_KEY_A)) {
		position += right * (float) (dTime * moveSpeed);
		this->camera->setDirty();
	}
	if (Input::get()->isKeyPressed(GLFW_KEY_W)) {
		position -= forward * (float) (dTime * moveSpeed);
		this->camera->setDirty();
	}
	if (Input::get()->isKeyPressed(GLFW_KEY_S)) {
		position += forward * (float) (dTime * moveSpeed);
		this->camera->setDirty();
	}
	if(Input::get()->isKeyPressed(GLFW_KEY_Q)) {
		position.y -= (float) (dTime * moveSpeed);
		this->camera->setDirty();
	}
	if(Input::get()->isKeyPressed(GLFW_KEY_E)) {
		position.y += (float) (dTime * moveSpeed);
		this->camera->setDirty();
	}
	
	glm::vec2 mousePos = Input::get()->getMousePos();
	glm::vec2 dMouse = mousePos - prevMousePos;
	if(dMouse.x != 0 || dMouse.y != 0) {
		rotation.y += dMouse.x * (float) dTime * rotSpeed;
		rotation.x += dMouse.y * (float) dTime * rotSpeed;
		rotation.x = std::clamp(rotation.x, glm::radians(-90.0f), glm::radians(90.0f));
		camera->setDirty();
	}
	
	prevMousePos = mousePos;
}
