#pragma once

// System
#include <memory>
#include <array>

// Dependencies
#include <glm/glm.hpp>

// Engine Files
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

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
	// texture management
	std::shared_ptr<Texture> whiteTex;
	std::shared_ptr<Texture> createWhiteTexture();

	// sprite-batching settings
	static const uint32_t MAX_TEXTURES = 32;
	static const uint32_t MAX_QUADS = 64; // quads per batch
	static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_INDICES = MAX_QUADS * 6;

	// sprite data containers
	Shader spriteShader;
	uint32_t textureSlotIdx = 1;
	std::array<std::shared_ptr<Texture>, MAX_TEXTURES> textures;
	std::shared_ptr<Mesh> dynMesh;
	std::shared_ptr<Mesh> createDynMesh();

	uint32_t quadIdx; // the quad we are currently constructing TODO: Move data into struct
	// considering rewriting this to map the buffer to a range instead of storing the data on the cpu.
	glm::vec3 vertexData[MAX_VERTICES]; // 1 vec3 per vertex
	float uvData[MAX_VERTICES * 2]; // 2 floats per vertex
	float colorData[MAX_VERTICES * 4]; // 4 floats per vertex
	float texIdData[MAX_VERTICES]; // 1 float per vertex (id)

	// line-batching settings
	static const uint32_t MAX_LINES = 16; // lines per batch
	static const uint32_t MAX_LINE_VERTS = MAX_LINES * 2;

	// line data containers
	Shader lineShader;
	uint32_t lineIdx;
	glm::vec2 lineVerts[MAX_LINE_VERTS];
	glm::vec4 lineColors[MAX_LINE_VERTS];
	std::shared_ptr<Mesh> lineMesh;

	// camera matrices
	glm::mat4 viewMat;
	glm::mat4 projMat;

	// batch management functions
	void checkQuadFlushConditions(std::shared_ptr<Texture> texture, float& textureIndex);
	void addQuad(glm::mat4 modelMat, glm::vec4 uvs, float textureIdx, glm::vec4 color);

	void checkLineFlushConditions();

	void flushQuads();
	void flushCircles();
	void flushLines();
public:
	Renderer2D();

	void clear();
	void setClearColor(float red, float green, float blue, float alpha);
	void setClearColor(glm::vec4 color);

	void startFrame(std::shared_ptr<Camera> camera);

	void renderQuad(glm::mat4 modelMat, std::shared_ptr<Texture> texture, glm::vec4 uvs = { 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	/*void renderSprite(Transform& transform, SpriteComponent& sprite);
	void renderSpriteSet(Transform& transform, std::shared_ptr<Spriteset> spriteset, int id, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void renderTilemap(Transform& transform, Tilemap& tilemap);*/

	void setLineWidth(float size) { glLineWidth(size); };
	void drawLine(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});
	void drawRect(glm::vec2 centerPos, glm::vec2 size, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});
	void drawCircle(glm::vec2 pos, float radius, float thickness = 1.0f, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});

	void endFrame();
	void flush();
};