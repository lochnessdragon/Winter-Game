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
    Camera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation);
    ~Camera();

    virtual void calculateViewMat();
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
private:
    glm::ivec2 fbSize;
public:
    OrthoCamera(std::shared_ptr<Window> window, glm::vec3 position, glm::vec3 rotation);
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
