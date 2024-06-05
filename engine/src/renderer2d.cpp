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
Renderer2D::Renderer2D() : whiteTex(createWhiteTexture()), spriteShader("res/shaders/sprite_2d.vert", "res/shaders/sprite_2d.frag"), lineShader("res/shaders/line.vert", "res/shaders/line.frag"), dynMesh(createDynMesh()), quadIdx(0), textureSlotIdx(1), lineIdx(0) {
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	textures[0] = whiteTex;

	spriteShader.use();
	int textureIds[32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	spriteShader.loadUniformArray("spriteTextures[0]", 32, textureIds);

	// create dynamic mesh for the lines
	lineMesh = std::make_shared<Mesh>(MeshUsage::Dynamic);
	lineMesh->attachBuffer(GL_FLOAT, 2, MAX_LINE_VERTS * (uint32_t)sizeof(glm::vec2), nullptr);
	lineMesh->attachBuffer(GL_FLOAT, 4, MAX_LINE_VERTS * (uint32_t)sizeof(glm::vec4), nullptr);
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

void Renderer2D::startFrame(std::shared_ptr<Camera> camera) {
	this->clear();
	// update view and projection matrix
	viewMat = camera->getViewMat();
	projMat = camera->getProjMat();
}

void Renderer2D::checkQuadFlushConditions(std::shared_ptr<Texture> texture, float& textureIndex) {
	if (quadIdx >= MAX_QUADS)
		flushQuads();

	if (!texture) {
		texture = whiteTex;
	}

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
			flushQuads();
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

void Renderer2D::renderQuad(glm::mat4 modelMat, std::shared_ptr<Texture> texture, glm::vec4 uvs, glm::vec4 color)
{
	float textureIndex = 0;
	checkQuadFlushConditions(texture, textureIndex);
	addQuad(modelMat, uvs, textureIndex, color);
}

void Renderer2D::checkLineFlushConditions() {
	if (lineIdx >= MAX_LINES)
		flushLines();
}

//void Renderer2D::renderSprite(Transform& transform, SpriteComponent& sprite) {
//	float textureIndex = 0;
//	checkQuadFlushConditions(sprite.texture, textureIndex);
//
//	glm::mat4 modelMat = transform.getModelMat();
//	addQuad(modelMat, sprite.uvs, textureIndex, sprite.color);
//}
//
//void Renderer2D::renderSpriteSet(Transform& transform, std::shared_ptr<Spriteset> spriteset, int id, glm::vec4 color) {
//	float textureIdx = 0;
//	checkQuadFlushConditions(spriteset->texture, textureIdx);
//	glm::vec4 uvs = spriteset->getUVCoords(id);
//
//	addQuad(transform.getModelMat(), uvs, textureIdx, color);
//}
//
//void Renderer2D::renderTilemap(Transform& transform, Tilemap& tilemap) {
//	int baseX = tilemap.getSpriteset()->tileSize / 2;
//	int baseY = tilemap.getSpriteset()->tileSize / 2;
//
//	for (int y = 0; y < tilemap.height; y++) {
//		for (int x = 0; x < tilemap.width; x++) {
//			int tileId = tilemap.get({ x, y });
//			
//			// skip empty
//			if (tileId >= 0) {
//				// we have to retrive the texture index each iteration to ensure we don't overflow the quad buffer
//				float textureIndex = 0;
//				checkQuadFlushConditions(tilemap.getSpriteset()->texture, textureIndex);
//
//				// this is an absolute block of a unit. Basically converts the tile position to an onscreen position in coords.
//				// this is very specific for an orthographic camera
//				// also, it doesn't support transforming the tilemap, which is defininitely a TODO!
//				glm::mat4 modelMat = glm::translate(glm::mat4(1), { 
//					((float)x * tilemap.getSpriteset()->tileSize) + (float) baseX, 
//					((float)y * tilemap.getSpriteset()->tileSize) + (float) baseY, 
//					0.0f }) * glm::scale(glm::mat4(1), glm::vec3(tilemap.getSpriteset()->tileSize / 2));
//
//				// finally, add the quad to the buffer
//				addQuad(modelMat, tilemap.getSpriteset()->getUVCoords(tileId), textureIndex, { 1,1,1,1 });
//			}
//		}
//	}
//}

void Renderer2D::drawLine(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
	checkLineFlushConditions();
	lineVerts[lineIdx] = pos1;
	lineVerts[lineIdx + 1] = pos2;

	lineColors[lineIdx] = color;
	lineColors[lineIdx + 1] = color;

	lineIdx += 2;
}

void Renderer2D::drawRect(glm::vec2 centerPos, glm::vec2 size, glm::vec4 color) {
	glm::vec2 halfSize = size / 2.0f;
	drawLine({ centerPos.x - halfSize.x, centerPos.y - halfSize.y }, { centerPos.x + halfSize.x, centerPos.y - halfSize.y }, color);
	drawLine({ centerPos.x + halfSize.x, centerPos.y - halfSize.y }, { centerPos.x + halfSize.x, centerPos.y + halfSize.y }, color);
	drawLine({ centerPos.x + halfSize.x, centerPos.y + halfSize.y }, { centerPos.x - halfSize.x, centerPos.y + halfSize.y }, color);
	drawLine({ centerPos.x - halfSize.x, centerPos.y + halfSize.y }, { centerPos.x - halfSize.x, centerPos.y - halfSize.y }, color);
}

void Renderer2D::drawCircle(glm::vec2 pos, float radius, float thickness, glm::vec4 color) {

}

void Renderer2D::flush() {
	if (quadIdx > 0)
		flushQuads();
	if (lineIdx > 0)
		flushLines();
}

void Renderer2D::flushQuads() {
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

void Renderer2D::flushLines() {
	lineShader.use();
	lineShader.loadUniform("VPMat", projMat * viewMat);

	lineMesh->bind();
	lineMesh->setBuffer(0, lineIdx * sizeof(glm::vec2), lineVerts);
	lineMesh->setBuffer(1, lineIdx * sizeof(glm::vec4), lineColors);
	glDrawArrays(GL_LINES, 0, lineIdx);
	lineMesh->unbind();

	lineIdx = 0;
}

void Renderer2D::flushCircles()
{
}

void Renderer2D::endFrame() {
	if(quadIdx > 0)
		flush();
}

