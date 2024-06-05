#include "camera.h"

// System files
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <log.h>
#include <mesh_generator.h>

// todo: tie camera to window so that the camera will update its aspect ratio with the window.
Camera::Camera(glm::vec3 position, glm::vec3 rotation) {
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

OrthoCamera::OrthoCamera(glm::ivec2 size, glm::vec3 position, glm::vec3 rotation) : Camera(position, rotation), fbSize(size) {
    this->calculateProjMat();
}

OrthoCamera::OrthoCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation) : Camera(position, rotation), fbSize(window->getSize()) {
    this->calculateProjMat();

    window->getWindowResizeHandler().addListener([=](const WindowResizeEventData& event) {
        this->fbSize = glm::ivec2(event.width, event.height);
        calculateProjMat();
        return false;
    });
}

OrthoCamera::~OrthoCamera() {}

void OrthoCamera::calculateProjMat() {
    projMat = glm::ortho(0.0f, (float) this->fbSize.x, 0.0f, (float) this->fbSize.y, -1.0f, 1.0f);
    //Log::getRendererLog()->trace("Projection Mat: {}\n Fb: {}", glm::to_string(projMat), glm::to_string(this->fbSize));
}

PixelPerfectCamera::PixelPerfectCamera(std::shared_ptr<Window> window, int width, int height, glm::vec3 position, glm::vec3 rotation) : OrthoCamera(glm::ivec2(width, height), position, rotation), window(window), framebuffer(std::make_shared<Framebuffer>(width, height)), modelMat(glm::mat4(1.0f)), mesh(createQuad()), screenShader("res/shaders/post/copy.vert", "res/shaders/post/copy.frag") {
    calculateSizing(window->getSize());
    
    window->getWindowResizeHandler().addListener([=](const WindowResizeEventData& event) {
        calculateSizing(glm::ivec2(event.width, event.height));

        return false;
    });
}

void PixelPerfectCamera::startScene() {
    framebuffer->bind();
    glViewport(0, 0, framebuffer->getSize().x, framebuffer->getSize().y);
}

void PixelPerfectCamera::endScene() {
    framebuffer->unbind();

    window->updateSize();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screenShader.use();
    screenShader.loadUniform("texture", 0);
    screenShader.loadUniform("modelMat", modelMat);

    mesh->bind();
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    framebuffer->bindColorBuffer();
    
    glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
}

void PixelPerfectCamera::calculateSizing(glm::ivec2 actual) {
    // calculate new rectangle coords + size
    float desiredRatio = (float) this->fbSize.x / (float) this->fbSize.y;
    float ratio = (float)actual.x / (float)actual.y;
    Log::getRendererLog()->info("Fb and cam ratios: fb= {} cam= {}", desiredRatio, ratio);

    float width = 1;
    float height = 1;
    if (ratio > desiredRatio) {
        // wider than tall
        width = desiredRatio * (1 / ratio);
    }
    else if (ratio < desiredRatio) {
        // taller than wide
        height = (1 / desiredRatio) * ratio;
    }

    Log::getRendererLog()->info("Calculated camera scaling at {} x {}", width, height);

    // calculate model mat
    modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));
}

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation, float aspect, float fov, float near, float far) : Camera(position, rotation), aspect(aspect), fov(fov), near(near), far(far) {
    this->calculateProjMat();
    
    window->getWindowResizeHandler().addListener([=](const WindowResizeEventData& event) {
        this->aspect = (float)event.width / (float)event.height;
        calculateProjMat();
        return false;
    });
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::calculateProjMat() {
    projMat = glm::perspective(this->fov, this->aspect, this->near, this->far);
}
