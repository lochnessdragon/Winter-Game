#pragma once

#include <camera.h>

class CameraController {
private:
    std::shared_ptr<Camera> camera;
	
	glm::vec2 prevMousePos;
public:
	float moveSpeed;
	float rotSpeed;
	
	CameraController();
    CameraController(std::shared_ptr<Camera> cam);
    
    void update(double deltaTime);
    
    std::shared_ptr<Camera> getCamera() { return camera; }
    const std::shared_ptr<Camera> getCamera() const { return camera; }
	
	void setCamera(std::shared_ptr<Camera> cam) {
		camera = cam;
	}
};
