#include "input.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "log.h"

Input* Input::singleton = nullptr;

std::unordered_map<std::string, int> Input::keynameToGLFW = {
	{"a", GLFW_KEY_A},
	{"d", GLFW_KEY_D},
	{"s", GLFW_KEY_S},
	{"w", GLFW_KEY_W},
	{"z", GLFW_KEY_Z},
	{"^", GLFW_KEY_UP},
	{"V", GLFW_KEY_DOWN},
	{"<", GLFW_KEY_LEFT},
	{">", GLFW_KEY_RIGHT}};

std::unordered_map<std::string, int> Input::gamepadBtnToGLFW = { 
	{"a", GLFW_GAMEPAD_BUTTON_A}, 
	{"b", GLFW_GAMEPAD_BUTTON_B}, 
	{"x", GLFW_GAMEPAD_BUTTON_X}, 
	{"y", GLFW_GAMEPAD_BUTTON_Y},
	{"left_bumper", GLFW_GAMEPAD_BUTTON_LEFT_BUMPER},
	{"right_bumper", GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER}};

float Input::stickAllowance = 0.05f;

Input::Input(GLFWwindow* window) {
	this->handle = window;
	glfwSetJoystickCallback(Input::joystickUpdate);

	Input::singleton = this;
}

void Input::joystickUpdate(int jid, int event) {
	const char* name = glfwGetJoystickName(jid);
	if (event == GLFW_CONNECTED)
	{
		// A joystick was connected
		Log::getGameLog()->trace("Joystick #{}: {} was connected.", jid, name);
		if (glfwJoystickIsGamepad(jid) == GLFW_TRUE) {
			const char* gamepad_name = glfwGetGamepadName(jid);
			Log::getGameLog()->trace("{} is gamepad \"{}\"", name, gamepad_name);
		}
	}
	else if (event == GLFW_DISCONNECTED)
	{
		// A joystick was disconnected
		Log::getGameLog()->trace("Joystick #{}: {} was disconnected.", jid, name);
	}
}

int Input::convertKeyboardNameToGLFWId(const std::string& id)
{
	auto it = keynameToGLFW.find(id);
	if (it != keynameToGLFW.end()) {
		return it->second;
	}
	else { throw std::runtime_error("Cannot find a key mapping for: " + id); }
}

int Input::convertControllerButtonNameToGLFWId(const std::string& id)
{
	auto it = gamepadBtnToGLFW.find(id);
	if (it != gamepadBtnToGLFW.end()) {
		return it->second;
	}
	else { throw std::runtime_error("Cannot find a gamepad mapping for: " + id); }
}

void Input::joystickInfo()
{
	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {
			const char* name = glfwGetJoystickName(i);
			Log::getGameLog()->info("Joystick {} is present (name: {})", i, name);
			if (glfwJoystickIsGamepad(i))
			{
				const char* gamepadName = glfwGetGamepadName(i);
				Log::getGameLog()->info("Joystick {} is also a gamepad named {}", i, gamepadName);
			}
		}
	}
}

bool Input::isKeyPressed(int key) {
	return glfwGetKey(handle, key) == GLFW_PRESS;
}

void Input::disableCursor() {
	glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::resetCursor() {
	glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

glm::vec2 Input::getMousePos() {
	double x_pos, y_pos;
	glfwGetCursorPos(handle, &x_pos, &y_pos);
	return glm::vec2(x_pos, y_pos);
}

bool Input::isMouseButtonPressed(int button) {
	return glfwGetMouseButton(handle, button) == GLFW_PRESS;
}

void Input::map(const std::string& filename)
{
	try {
		std::fstream file(filename);
		nlohmann::json data = nlohmann::json::parse(file);
		file.close();

		if (!data.is_object()) {
			throw std::runtime_error("Root of mapping file is non-object type");
		}

		for (auto& [name, mapping] : data.items()) {
			Log::getGameLog()->info("Processing Mapping: {}", name);
			auto type = mapping.at("type");
			if (type == "normalized_vec2") {
				Vector2Mapping vec2Mapping;
				
				// keyboard mappings
				for (auto key : mapping.at("keyboard")) {
					KeyboardMapping keyMapping;
					keyMapping.posAxis1 = convertKeyboardNameToGLFWId(key.at("axis_1").at("pos").template get<std::string>());
					keyMapping.negAxis1 = convertKeyboardNameToGLFWId(key.at("axis_1").at("neg").template get<std::string>());
					keyMapping.posAxis2 = convertKeyboardNameToGLFWId(key.at("axis_2").at("pos").template get<std::string>());
					keyMapping.negAxis2 = convertKeyboardNameToGLFWId(key.at("axis_2").at("neg").template get<std::string>());
					vec2Mapping.keyboardMappings.push_back(keyMapping);
				}

				// gamepad mappings
				for (auto key : mapping.at("gamepad")) {
					std::string axisName = key.template get<std::string>();
					if (axisName == "left_axis") {
						vec2Mapping.gamepadAxes |= (unsigned char) GamepadAxisVec2::LeftStick;
					}
					else if (axisName == "right_axis") {
						vec2Mapping.gamepadAxes |= (unsigned char) GamepadAxisVec2::RightStick;
					}
					else if (axisName == "dpad") {
						vec2Mapping.gamepadAxes |= (unsigned char) GamepadAxisVec2::DPad;
					}
					else {
						throw std::runtime_error("Couldn't map gamepad axis from: " + axisName);
					}
				}

				this->vec2Mappings.emplace(name, vec2Mapping);
			}
			else if (type == "button") {
				ButtonMapping buttonMapping;

				for (auto key : mapping.at("keyboard")) {
					buttonMapping.keyboardMappings.push_back(convertKeyboardNameToGLFWId(key.template get<std::string>()));
				}

				for (auto gamepadBtn : mapping.at("gamepad")) {
					buttonMapping.gamepadMappings.push_back(convertControllerButtonNameToGLFWId(gamepadBtn.template get<std::string>()));
				}

				this->buttonMappings.emplace(name, buttonMapping);
			}
		}
	}
	catch (std::exception& e) {
		Log::getGameLog()->error("Failed to load mappings from file: {}. ERROR: {}", filename, e.what());
	}
}

bool Input::getInput(const std::string& name)
{
	ButtonMapping mapping = this->buttonMappings.at(name);

	// check keyboard first
	bool result = false;

	for (int key : mapping.keyboardMappings) {
		if (isKeyPressed(key)) {
			result = true;
			break;
		}
	}

	if (!result) {
		for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
			if (result)
				break;
			if (glfwJoystickPresent(i)) {
				if (glfwJoystickIsGamepad(i)) {
					GLFWgamepadstate state;
					glfwGetGamepadState(i, &state);

					for (int gamepadKey : mapping.gamepadMappings) {
						result = state.buttons[gamepadKey];
						if (result)
							break;
					}
				}
			}
		}
	}

	return result;
}

glm::vec2 Input::getInputAxis(const std::string& name)
{
	Vector2Mapping mapping = this->vec2Mappings.at(name);

	glm::vec2 input = glm::vec2(0.0f);
	
	// check keyboard first

	// this is really idiotic code and prime for refactoring
	bool posAxis1 = false;
	bool negAxis1 = false;
	bool posAxis2 = false;
	bool negAxis2 = false;

	for (KeyboardMapping keyMap : mapping.keyboardMappings) {
		if (isKeyPressed(keyMap.posAxis1))
			posAxis1 = true;
		if (isKeyPressed(keyMap.negAxis1))
			negAxis1 = true;
		if (isKeyPressed(keyMap.posAxis2))
			posAxis2 = true;
		if (isKeyPressed(keyMap.negAxis2))
			negAxis2 = true;
	}

	if (posAxis1) {
		input.x += 1.0f;
	}
	if (negAxis1) {
		input.x -= 1.0f;
	}
	if (posAxis2) {
		input.y += 1.0f;
	}
	if (negAxis2) {
		input.y -= 1.0f;
	}

	// then check controller input (only if keyboard input is wack)
	if (!posAxis1 && !negAxis1 && !posAxis2 && !negAxis2) {
		bool updated = false;
		for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
			if (updated)
				break;
			if (glfwJoystickPresent(i)) {
				if (glfwJoystickIsGamepad(i)) {
					GLFWgamepadstate state;
					glfwGetGamepadState(i, &state);
					if (mapping.gamepadAxes & (unsigned char) GamepadAxisVec2::LeftStick) {
						// check left axis
						float x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
						float y = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]; // invert
						
						if (abs(x) > Input::stickAllowance) {
							input.x = x;
							updated = true;
						}

						if (abs(y) > Input::stickAllowance) {
							input.y = y;
							updated = true;
						}
					}

					if (mapping.gamepadAxes & (unsigned char)GamepadAxisVec2::RightStick && !updated) {
						// check right axis
						float x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
						float y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]; // invert

						if (abs(x) > Input::stickAllowance) {
							input.x = x;
							updated = true;
						}

						if (abs(y) > Input::stickAllowance) {
							input.y = y;
							updated = true;
						}
					}

					if (mapping.gamepadAxes & (unsigned char)GamepadAxisVec2::DPad && !updated) {
						// check dpad
						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS) {
							input.x -= 1.0f;
							updated = true;
						}

						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS) {
							input.x += 1.0f;
							updated = true;
						}

						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS) {
							input.y -= 1.0f;
							updated = true;
						}

						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS) {
							input.y += 1.0f;
							updated = true;
						}
					}
				}
			}
		}
	}

	return input;
}

void Input::update() {
	// needs to happen every frame
	glfwPollEvents();
}
