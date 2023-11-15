#pragma once

// System
#include <memory>

// Dependencies
#include <glm/glm.hpp>

// Engine Files
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "scene/transform.h"

/**
* We are reimplementing a lot of the 3d renderer methods. A better idea would be to have a base renderer class with derived rendering functionality.
* The base class handles overall stuff, such as clear color, clearing frame, opengl state management, whilst the renderers handle their specific subset of graphics features
* 
* Features:
*  - Batched Rendering
* 
* Planned:
*  - Interleaving (better performance (allegedly)
*/
class Renderer2D {
private:
	std::shared_ptr<Texture> whiteTex;
	std::shared_ptr<Texture> createWhiteTexture();

	static const uint32_t MAX_QUADS = 64; // quads per batch
	static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_INDICES = MAX_QUADS * 6;

	Shader spriteShader;

	std::shared_ptr<Mesh> dynMesh;
	std::shared_ptr<Mesh> createDynMesh();

	uint32_t quadIdx; // the quad we are currently constructing
	glm::vec3 vertexData[MAX_VERTICES]; // 1 vec3 per vertex
	float uvData[MAX_VERTICES * 2]; // 2 floats per vertex
	float colorData[MAX_VERTICES * 4]; // 4 floats per vertex

	// matrices
	glm::mat4 viewMat;
	glm::mat4 projMat;

	std::shared_ptr<Texture> currTex;
public:
	Renderer2D();

	void clear();
	void setClearColor(float red, float green, float blue, float alpha);
	void setClearColor(glm::vec4 color);

	void startFrame();

	void renderSprite(std::shared_ptr<Camera> camera, Transform transform);
	void renderSprite(std::shared_ptr<Camera> camera, Transform transform, std::shared_ptr<Texture> texture, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});

	void endFrame();
	void flush();
};