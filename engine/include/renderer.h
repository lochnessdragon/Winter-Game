#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "object.h"
#include "light.h"

/**
 * Things we need to support graphically
 *  - Instancing
 *  - Sorting object back to front
 *  - different shaders and materials
 *  - multiple lights
 *  - multiple light types
 *  - multiple different textures/materials on one model and a way to shift between them
 *  - normals maps
 *  - metallic maps
 *  - pbr materials
 *  - figure out an art style
 *  - shadows
 *  - reflections (at some point)
 */

class Renderer {
public:
	Renderer();

	void clear();
	void setClearColor(float red, float green, float blue, float alpha);
	void setClearColor(glm::vec4 color);
	
	void startFrame();
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Object> object, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, const AmbientLight& ambientLight, const Light& light);
    
    void toggleWireframe();

private:
	bool wireframeOn;
};
