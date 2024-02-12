#include "renderer2d.h"

#include "platform.h"
#include "GLAPI.h"

#include "log.h"

#include <glm/gtx/string_cast.hpp>

/**
* Triangle Idxs
* [0] = Top-left
* [1] = Top-right
* [2] = Bottom-left
* [3] = Bottom-right
*/

// don't know if I love the white texture's initialization. It's concise, but not readable
Renderer2D::Renderer2D() : whiteTex(createWhiteTexture()), spriteShader("res/shaders/sprite_2d.vert", "res/shaders/sprite_2d.frag"), dynMesh(createDynMesh()), quadIdx(0), textureSlotIdx(1) {
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	textures[0] = whiteTex;

	spriteShader.use();
	int textureIds[32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	spriteShader.loadUniformArray("spriteTextures[0]", 32, textureIds);
}

std::shared_ptr<Texture> Renderer2D::createWhiteTexture() {
	TextureSpecification spec;
	spec.height = 1;
	spec.width = 1;
	unsigned char data[4] = {255, 255, 255, 255};
	return std::make_shared<Texture>(spec, data);
}

std::shared_ptr<Mesh> Renderer2D::createDynMesh() {
	// generate indices [0, 1, 2, 0, 2, 3] = 1 triangle
	unsigned int indices[MAX_INDICES] = { 0 };
	for (unsigned int i = 0; i < (MAX_INDICES / 6); i++) {
		unsigned int real_idx = i * 6;
		unsigned int base_index = i * 4;
		indices[real_idx] = base_index;
		indices[real_idx + 1] = base_index + 2;
		indices[real_idx + 2] = base_index + 1;
		indices[real_idx + 3] = base_index + 1;
		indices[real_idx + 4] = base_index + 2;
		indices[real_idx + 5] = base_index + 3;
	}

	auto mesh = std::make_shared<Mesh>(MAX_INDICES * (uint32_t) sizeof(int), indices, MAX_VERTICES * (uint32_t) sizeof(glm::vec3), nullptr, MAX_VERTICES * 2 * (uint32_t) sizeof(float), nullptr, MeshUsage::Dynamic); // I am assured OpenGL doesn't copy when a nullptr is passed to glBufferData
	mesh->attachBuffer(GL_FLOAT, 4, MAX_VERTICES * 4 * sizeof(float), nullptr); // color buffer
	mesh->attachBuffer(GL_FLOAT, 1, MAX_VERTICES * sizeof(float), nullptr); // texture id buffer
	return mesh;
}

void Renderer2D::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer2D::setClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer2D::setClearColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::startFrame() {
	this->clear();
}

//void Renderer2D::renderSprite(std::shared_ptr<Camera> camera, Transform& transform) {
//	renderSprite(camera, transform, whiteTex);
//}

void Renderer2D::checkFlushConditions(std::shared_ptr<Camera> camera, std::shared_ptr<Texture> texture, float& textureIndex) {
	if (quadIdx >= MAX_QUADS)
		flush();

	if (!texture) {
		texture = whiteTex;
	}

	// skip non-consistency checks if this is our first quad
	if (quadIdx > 0) {
		if (viewMat != camera->getViewMat() || projMat != camera->getProjMat()) {
			flush();
		}
	}

	// update view and projection matrix
	viewMat = camera->getViewMat();
	projMat = camera->getProjMat();

	textureIndex = -1.0f;
	for (uint32_t i = 1; i < textureSlotIdx; i++)
	{
		if (textures[i] == texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex < 0) {
		if (textureSlotIdx >= MAX_TEXTURES) {
			flush();
		}

		textureIndex = (float) textureSlotIdx;
		textures[textureSlotIdx] = texture;
		textureSlotIdx++;
	}
}

void Renderer2D::addQuad(glm::mat4 modelMat, glm::vec4 uvs, float textureIdx, glm::vec4 color) {
	uint32_t vertex_idx = quadIdx * 4;

	// quad vertices
	vertexData[vertex_idx] = modelMat * glm::vec4(-1.0, 1.0, 0.0, 1.0);
	vertexData[vertex_idx + 1] = modelMat * glm::vec4(1.0, 1.0, 0.0, 1.0);
	vertexData[vertex_idx + 2] = modelMat * glm::vec4(-1.0, -1.0, 0.0, 1.0);
	vertexData[vertex_idx + 3] = modelMat * glm::vec4(1.0, -1.0, 0.0, 1.0);

	uint32_t uv_idx = quadIdx * 4 * 2;

	// top-left
	uvData[uv_idx] = uvs.x;
	uvData[uv_idx + 1] = uvs.w;

	// top-right
	uvData[uv_idx + 2] = uvs.z;
	uvData[uv_idx + 3] = uvs.w;

	// bottom-left
	uvData[uv_idx + 4] = uvs.x;
	uvData[uv_idx + 5] = uvs.y;

	// bottom-right
	uvData[uv_idx + 6] = uvs.z;
	uvData[uv_idx + 7] = uvs.y;

	// texture ids
	texIdData[vertex_idx] = textureIdx;
	texIdData[vertex_idx + 1] = textureIdx;
	texIdData[vertex_idx + 2] = textureIdx;
	texIdData[vertex_idx + 3] = textureIdx;

	uint32_t color_idx = quadIdx * 4 * 4;
	for (uint32_t i = 0; i < 4; i++) {
		colorData[color_idx + (i * 4)] = color.x;
		colorData[color_idx + (i * 4) + 1] = color.y;
		colorData[color_idx + (i * 4) + 2] = color.z;
		colorData[color_idx + (i * 4) + 3] = color.w;
	}

	quadIdx++;
}

void Renderer2D::renderSprite(std::shared_ptr<Camera> camera, Transform& transform, SpriteComponent& sprite) {
	float textureIndex = 0;
	checkFlushConditions(camera, sprite.texture, textureIndex);

	glm::mat4 modelMat = transform.getModelMat();
	addQuad(modelMat, sprite.uvs, textureIndex, sprite.color);
}

void Renderer2D::renderSpriteSet(std::shared_ptr<Camera> camera, Transform& transform, std::shared_ptr<Spriteset> spriteset, int id, glm::vec4 color) {
	float textureIdx;
	checkFlushConditions(camera, spriteset->texture, textureIdx);
	glm::vec4 uvs = spriteset->getUVCoords(id);

	addQuad(transform.getModelMat(), uvs, textureIdx, color);
}

void Renderer2D::renderTilemap(std::shared_ptr<Camera> camera, Transform& transform, Tilemap& tilemap) {
	int baseX = tilemap.getSpriteset()->tileSize / 2;
	int baseY = tilemap.getSpriteset()->tileSize / 2;
	for (int y = 0; y < tilemap.height; y++) {
		for (int x = 0; x < tilemap.width; x++) {
			int tileId = tilemap.get({ x, y });
			// skip empty
			if (tileId >= 0) {
				float textureIndex = 0;
				checkFlushConditions(camera, tilemap.getSpriteset()->texture, textureIndex);
				// this is an absolute block of a unit. Basically converts the tile position to an onscreen position in coords.
				glm::mat4 modelMat = glm::translate(glm::mat4(1), { 
					((float)x * tilemap.getSpriteset()->tileSize) + (float) baseX, 
					((float)y * tilemap.getSpriteset()->tileSize) + (float) baseY, 
					0.0f }) * glm::scale(glm::mat4(1), glm::vec3(tilemap.getSpriteset()->tileSize / 2));
				addQuad(modelMat, tilemap.getSpriteset()->getUVCoords(tileId), textureIndex, { 1,1,1,1 });
			}
		}
	}
}

void Renderer2D::drawRect(glm::vec2 centerPos, glm::vec2 size) {
	Log::getRendererLog()->warn("Draw rect not implemented: <{}, {}>", glm::to_string(centerPos), glm::to_string(size));
}
void Renderer2D::drawCircle(glm::vec2 pos, float radius) {
	Log::getRendererLog()->warn("Draw circle not implemented: <{}, {}>", glm::to_string(pos), radius);
}

void Renderer2D::flush() {
	spriteShader.use();
	spriteShader.loadUniform("VPMat", projMat * viewMat);

	for (uint32_t i = 0; i < textureSlotIdx; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->bind();
	}

	dynMesh->bind();
	dynMesh->setBuffer(0, quadIdx * 4 * sizeof(glm::vec3), vertexData);
	dynMesh->setBuffer(1, quadIdx * 4 * 2 * sizeof(float), uvData);
	dynMesh->setBuffer(2, quadIdx * 4 * 4 * sizeof(float), colorData);
	dynMesh->setBuffer(3, quadIdx * 4 * sizeof(float), texIdData);
	glDrawElements(GL_TRIANGLES, quadIdx * 6, GL_UNSIGNED_INT, 0);
	dynMesh->unbind();

	quadIdx = 0;
	textureSlotIdx = 1; // 0 is white texture
}

void Renderer2D::endFrame() {
	if(quadIdx > 0)
		flush();
}

