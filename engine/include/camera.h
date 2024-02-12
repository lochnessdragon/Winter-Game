#pragma once


#ifdef _MSC_VER
#pragma warning(disable: 4201) // disable warning 4201 from glm
#endif
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#ifdef _MSC_VER
#pragma warning(default: 4201) // enable warning 4201 back
#endif

#include "window.h"
#include "framebuffer.h"
#include "shader.h"
#include "mesh.h"
#include <memory>

class Camera {
protected:
    glm::mat4 viewMat;
    glm::mat4 projMat;

    glm::vec3 m_position;
	glm::quat rotation;
    glm::vec3 m_eulerRotation;
	
	bool dirty;
public:
    Camera(glm::vec3 position, glm::vec3 rotation);
    ~Camera();

    void calculateViewMat();
    virtual void calculateProjMat() = 0;

    glm::vec3& position() { return m_position; }
	const glm::vec3& position() const { return m_position; }
    glm::vec3& eulerRotation() { return m_eulerRotation; }
	const glm::vec3& eulerRotation() const { return m_eulerRotation; }
	glm::quat getQuatRotation() { return rotation; }
	
	// MUST be called after modifying the position or eulerRot.
	void setDirty() { dirty = true; }

	glm::mat4 getViewMat();
    glm::mat4 getProjMat() { return projMat; }
};

class OrthoCamera : public Camera {
protected:
    glm::ivec2 fbSize;
public:
    OrthoCamera(glm::ivec2 size, glm::vec3 position, glm::vec3 rotation);
    OrthoCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation);
    ~OrthoCamera();

    void calculateProjMat();
};

class PixelPerfectCamera : public OrthoCamera {
private:
    std::shared_ptr<Framebuffer> framebuffer;
    std::shared_ptr<Mesh> mesh;
    Shader screenShader;
    glm::mat4 modelMat;

    std::shared_ptr<Window> window;
public:
    PixelPerfectCamera(std::shared_ptr<Window> window, int width, int height, glm::vec3 position, glm::vec3 rotation);

    void startScene();
    void endScene();

    void calculateSizing(glm::ivec2 actual);

    std::shared_ptr<Framebuffer> getFBO() { return framebuffer; };
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
