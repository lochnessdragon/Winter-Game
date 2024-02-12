#include "ui/text-renderer.h"

#include "log.h"

#include "mesh_generator.h"

/**
* Triangle Idxs
* [0] = Top-left
* [1] = Top-right
* [2] = Bottom-left
* [3] = Bottom-right
*/

TextRenderer::TextRenderer(std::shared_ptr<Surface> surface) : textShader("res/shaders/glyph.vert", "res/shaders/glyph.frag"), dynamicMesh(createDynMesh()), quadIndex(0), vertexData(), uvData()
{
	glm::ivec2 size = surface->getSize();
	this->orthoMat = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, -1.0f, 1.0f);

	// tbh, this was an inelegant solution at best.
	/*window->getWindowResizeHandler().addListener([this](const WindowResizeEventData& data) {
		this->orthoMat = glm::ortho(0.0f, (float) data.width, (float) data.height, 0.0f, -1.0f, 1.0f);
		return false;
	});*/
}


void TextRenderer::render(const std::string& message, const std::weak_ptr<Font> font, glm::dvec2 pos, double scale, glm::vec4 color)
{
	// set color (flush if poopy)
	if (quadIndex > 0 && color != currColor) {
		flush();
	}
	currColor = color;

	// set font (FLUSH IF POOOOOPY!!!!)
	if (quadIndex > 0 && currFont.lock() && *currFont.lock().get() != *font.lock().get()) { // this could be the site of a memory issue
		flush();
	}
	currFont = font;

	double x = pos.x;
	double y = pos.y;

	// add more vertex information
	/* For one glyph, we need to know:
	*	(1) UV Coords
	*	(2) Position of the 4 vertices on screen
	*		(a) Spacing info between glyphs on-screen
	*		(b) Baseline info (How far above the baseline?)
	*  Thankfully!?! This is all handled by the font class.
	*/

	const msdf_atlas::FontGeometry& fontInfo = font.lock()->getFontGeometry();
	msdfgen::FontMetrics metrics = fontInfo.getMetrics();

	double spaceAdvance = fontInfo.getGlyph(' ')->getAdvance() * scale;

	// store the ratio between pixel coordinates and uv coordinates
	double pxToUvWidthRatio = 1.0 / font.lock()->getAtlasWidth();
	double pxToUvHeightRatio = 1.0 / font.lock()->getAtlasHeight();

	for (uint32_t i = 0; i < message.size(); i++) {
		// reached end of buffer?
		if (quadIndex >= MAX_QUADS) {
			// clear buffer
			flush();
		}

		char c = message[i];
		
		if (c == '\r')
			continue; // skip carriage return
		if (c == '\n') {
			// newline, reset x and update y
			x = 0.0;
			y += metrics.lineHeight * scale;
			continue;
		}
		if (c == ' ') {
			// space character, add a little to x, and grab the next character
			x += spaceAdvance;
			continue;
		}

		auto glyph = fontInfo.getGlyph(c);
		
		const uint32_t floatIdxStart = quadIndex * 4 * 2; // 4 vertices per quad, 2 floats per vertex

		// grab vertex coordinates and store in buffer
		// don't forget ccw winding order
		double vx_min, vy_min, vx_max, vy_max;
		// left is smallest x, right is largest x
		// however, the top is the smallest y, whilst the bottom is the largest y, because we are defining 0,0 as the top left of the screen
		// not the bottom left, like some plebians!
		glyph->getQuadPlaneBounds(vx_min, vy_max, vx_max, vy_min);
		vx_min = vx_min * scale;
		vy_min = vy_min * scale;
		vx_max = vx_max * scale;
		vy_max = vy_max * scale;

		// correct for improper coordinate system:
		// ^ y                  +--> x
		// |                    |
		// +--> x  (msdf)  vs.  v y     (ours)
		vy_min = -vy_min;
		vy_max = -vy_max;

		// top left
		vertexData[floatIdxStart] = (float) (vx_min + x);
		vertexData[floatIdxStart + 1] = (float) (vy_min + y);
		
		// top right
		vertexData[floatIdxStart + 2] = (float) (vx_max + x);
		vertexData[floatIdxStart + 3] = (float) (vy_min + y);

		// bottom left
		vertexData[floatIdxStart + 4] = (float) (vx_min + x);
		vertexData[floatIdxStart + 5] = (float) (vy_max + y);

		// bottom right
		vertexData[floatIdxStart + 6] = (float) (vx_max + x);
		vertexData[floatIdxStart + 7] = (float) (vy_max + y);

		// grab uv coordinates and store in buffer
		double uvx_min, uvy_min, uvx_max, uvy_max;
		glyph->getQuadAtlasBounds(uvx_min, uvy_min, uvx_max, uvy_max);
		glm::dvec2 uvCoordMin(uvx_min * pxToUvWidthRatio, uvy_min * pxToUvHeightRatio);
		glm::dvec2 uvCoordMax(uvx_max * pxToUvWidthRatio, uvy_max * pxToUvHeightRatio);
		// top left
		uvData[floatIdxStart] = (float) uvCoordMin.x;
		uvData[floatIdxStart + 1] = (float) uvCoordMax.y;

		// top right
		uvData[floatIdxStart + 2] = (float) uvCoordMax.x;
		uvData[floatIdxStart + 3] = (float) uvCoordMax.y;

		// bottom left
		uvData[floatIdxStart + 4] = (float) uvCoordMin.x;
		uvData[floatIdxStart + 5] = (float) uvCoordMin.y;

		// bottom right
		uvData[floatIdxStart + 6] = (float) uvCoordMax.x;
		uvData[floatIdxStart + 7] = (float) uvCoordMin.y;

		quadIndex++;

		if (i < message.size() - 1) {
			char next_c = message[i + 1];
			double deltaAdvance = 0;
			fontInfo.getAdvance(deltaAdvance, c, next_c);
			x += deltaAdvance * scale;
		}
	}
}

void TextRenderer::flush() {
	textShader.use();
	textShader.loadUniform("projMat", orthoMat);
	textShader.loadUniform("textColor", currColor);
	textShader.loadUniform("msdfAtlas", 0);

	glActiveTexture(GL_TEXTURE0);
	if (std::shared_ptr<Font> font = this->currFont.lock()) {
		font->bindAtlas();
	}
	else {
		return; // lost the font? no worries
	}

	dynamicMesh->bind();
	// upload mesh info
	dynamicMesh->setBuffer(0, quadIndex * 4 * 2 * sizeof(float), vertexData);
	dynamicMesh->setBuffer(1, quadIndex * 4 * 2 * sizeof(float), uvData);
	glDrawElements(GL_TRIANGLES, quadIndex * 6, GL_UNSIGNED_INT, 0);
	dynamicMesh->unbind();

	quadIndex = 0;
}

void TextRenderer::endFrame() {
	if (quadIndex > 0)
		flush();
}

std::shared_ptr<Mesh> TextRenderer::createDynMesh() {
	// generate indices [0, 1, 2, 0, 2, 3] = 1 triangle
	unsigned int indices[MAX_INDICES] = {0};
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

	return std::make_shared<Mesh>(MAX_INDICES, indices, MAX_VERTICES * 2, nullptr, MAX_VERTICES * 2, nullptr, MeshUsage::Dynamic); // I am assured OpenGL doesn't copy when a nullptr is passed to glBufferData
}
