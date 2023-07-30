#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "memory"

class Object {
private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    // std::shared_ptr<Mesh> mesh;

    glm::mat4 modelMat;

    bool dirty;

protected:
    void calculateModelMat();

public:
    Object();
    Object(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);//, std::shared_ptr<Mesh> mesh);
    ~Object();

    // this method should really be called at the end of a tick, not the beginning
    // void tick() { if(dirty) { calculateModelMat(); dirty = false; } }

    void setDirty() { dirty = true; }

    glm::vec3 getPosition() { return position; }
    glm::vec3 getScale() { return scale; }
    glm::vec3 getRotation() { return rotation; }

    void setPosition(glm::vec3 newPos) { this->position = newPos; setDirty(); }
    void setScale(glm::vec3 newScale) { this->scale = newScale; setDirty(); }
    void setRotation(glm::vec3 newRot) { this->rotation = newRot; setDirty(); }

    glm::mat4 getModelMat() { if(dirty) { calculateModelMat(); dirty = false; } return modelMat; }
};
