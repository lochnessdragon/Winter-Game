#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "object.h"

class Renderer {
public:
	Renderer();

	void clear();
	void setClearColor(float red, float green, float blue, float alpha);
	void setClearColor(glm::vec4 color);
	
	void startFrame();
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Object> object, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);

private:
};