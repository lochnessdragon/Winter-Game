#include "tilemap_editor.h"

#include <imgui.h>
#include <input.h>
#include <log.h>

#include <cmath>

#include <glm/gtx/string_cast.hpp>
#include <misc/cpp/imgui_stdlib.h>

TilemapEditor::TilemapEditor(std::string filename, std::shared_ptr<Window> win, std::shared_ptr<Tilemap> tilemap) : window(win), currentBrushId(0), filename(filename), tilemap(tilemap) {
	Input::get()->getKeyEventHandler().addListener([this](const KeyEventData& data) {
		if (data.action == GLFW_PRESS && data.key == GLFW_KEY_T) {
			active = !active;
			return true;
		}
		
		return false;
	});
}

void TilemapEditor::update() {
	if (active) {
		if (Input::get()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			// set the tile id for the position we are over

			glm::ivec2 screenSize = window->getSize();

			glm::vec2 mousePos = Input::get()->getMousePos();
			glm::ivec2 tilePos = { (mousePos.x / (float) screenSize.x) * tilemap->width, ((((float) screenSize.y) - mousePos.y) / (float) screenSize.y) * tilemap->height };
			//Log::getGameLog()->trace("Modifying: {}", glm::to_string(tilePos));
			if (tilemap->isInBounds(tilePos))
				tilemap->set(tilePos, currentBrushId);
		}
	}
}

void TilemapEditor::imgui() {
	ImGui::Begin("Tilemap Edit");
	ImGui::Text("Press T to edit the tilemap");
	if (ImGui::Button("Save")) {
		tilemap->save(filename);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		tilemap->load(filename);
	}

	ImGui::InputText("Filename", &this->filename);
	if (currentBrushId >= 0) {
		glm::vec4 uvCoords = tilemap->getSpriteset()->getUVCoords(currentBrushId);
		ImGui::Image((ImTextureID)tilemap->getSpriteset()->texture->getHandle(), { 16, 16 }, ImVec2(uvCoords.x, uvCoords.y), ImVec2(uvCoords.z, uvCoords.w));
	}
	else {
		ImGui::SetCursorPosX(16);
	}

	ImGui::SameLine();
	if (ImGui::BeginCombo("Tiles Combo", "Clear")) {
		for (int i = 0; i < (tilemap->getSpriteset()->getMaxId() + 1); i++) {
			const bool is_selected = (currentBrushId + 1 == i);
			std::string label = "##brush_" + std::to_string(i);

			if (ImGui::Selectable(label.c_str(), is_selected)) {
				currentBrushId = i - 1;
			}

			if (i > 0) {
				ImGui::SameLine();
				glm::vec4 uvCoords = tilemap->getSpriteset()->getUVCoords(i - 1);
				ImGui::Image((ImTextureID)tilemap->getSpriteset()->texture->getHandle(), { 16, 16 }, ImVec2(uvCoords.x, uvCoords.y), ImVec2(uvCoords.z, uvCoords.w));
			}
			ImGui::SameLine();
			std::string info = "Tile: " + std::to_string(i);
			if (i == 0)
				info = "Clear";
			ImGui::Text(info.c_str());

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::End();

	currentBrushId = std::min((int) tilemap->getSpriteset()->getMaxId() - 1, currentBrushId);
	currentBrushId = std::max(-1, currentBrushId);
}