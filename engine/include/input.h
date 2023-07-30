#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class InputType {
    KEYBOARD,
    CONTROLLER
};

class Input {
private:
	static GLFWwindow* handle;
public:
	static void setHandle(GLFWwindow* handle);
	
	static bool isKeyPressed(int key);
	
	static void disableCursor();
	static void resetCursor();
	static glm::vec2 getMousePos();
	static bool isMouseButtonPressed(int button);
	
	//static float getInput(const std::string& name); // getInput("move_z")
	
	static void update() { glfwPollEvents(); };
};
