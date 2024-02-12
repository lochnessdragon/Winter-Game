#pragma once

#include "GLAPI.h"
#include <glm/glm.hpp>

#include <event.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <memory>

enum class InputType {
    Keyboard,
    Controller
};

enum class ControllerType {
	Xbox,
	Playstation,
	Switch,
	Unknown
};

//enum class MappingType {
//	NormalizedVector2,
//	Button
//};
//
//struct InputMapping {
//	MappingType type;
//};

enum class GamepadAxisVec2 {
	LeftStick = 1,
	RightStick = 2,
	DPad = 4

};

struct KeyboardMapping {
	int posAxis1;
	int negAxis1;
	int posAxis2;
	int negAxis2;
};

struct ButtonMapping {
	std::vector<int> keyboardMappings;
	std::vector<int> gamepadMappings;
};

struct Vector2Mapping {
	std::vector<KeyboardMapping> keyboardMappings;
	unsigned char gamepadAxes = 0; // bitmap representing gamepad axes
};

struct KeyEventData {
	int key;
	int scancode;
	int action;
	int mods;
};

struct MouseButtonEventData {
	int button;
	int action;
	int mods;
};

class Input {
private:
	GLFWwindow* handle;

	static Input* singleton;

	// Event dispatchers
	EventDispatcher<KeyEventData> keyEvent;
	EventDispatcher<glm::vec2> mouseMoveEvent;
	EventDispatcher<MouseButtonEventData> mouseButtonEvent;

	// mappings
	std::unordered_map<std::string, ButtonMapping> buttonMappings;
	std::unordered_map<std::string, Vector2Mapping> vec2Mappings;

	// strings to enums 
	static std::unordered_map<std::string, int> keynameToGLFW;
	static std::unordered_map<std::string, int> gamepadBtnToGLFW;

	static int convertKeyboardNameToGLFWId(const std::string& id);
	static int convertControllerButtonNameToGLFWId(const std::string& id);

	static float stickAllowance; // maximum amount of axis movement before we actually accept it
public:
	Input(GLFWwindow* win);
	~Input() { if (singleton == this) { singleton = nullptr; } };

	// joystick functions
	static void joystickUpdate(int jid, int event);
	void joystickInfo();

	// keyboard functionss
	bool isKeyPressed(int key);
	
	// mouse functions
	void disableCursor();
	void resetCursor();
	glm::vec2 getMousePos();
	bool isMouseButtonPressed(int button);
	
	void map(const std::string& filename); // load input mappings
	bool getInput(const std::string& name);
	glm::vec2 getInputAxis(const std::string& name); // get vec2 mapping for input
	//static float getInput(const std::string& name); // getInput("move_z")

	// event dispatcher get functions
	auto& getKeyEventHandler() { return keyEvent; };
	auto& getMouseMoveEventHandler() { return mouseMoveEvent; };
	auto& getMouseButtonEventHandler() { return mouseButtonEvent; };
	
	void update();

	static Input* get() { if (!singleton) { throw std::runtime_error("Singleton doesn't exist for input class"); }; return singleton; };
};
