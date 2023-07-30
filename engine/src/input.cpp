#include "input.h"
#include "log.h"

GLFWwindow* Input::handle = 0;

#ifdef DEBUG
#define INPUT_DEBUG_CHECK if(Input::handle != 0) { \
	Log::getRendererLog()->error("Input::setHandle has not been called"); \
	throw std::runtime_error("Input::isKeyPressed called before the input system was setup"); \
}
#else
#define INPUT_DEBUG_CHECK
#endif

void Input::setHandle(GLFWwindow *handle) {
	Input::handle = handle;
}

bool Input::isKeyPressed(int key) {
	INPUT_DEBUG_CHECK;
	return glfwGetKey(Input::handle, key) == GLFW_PRESS;
}

void Input::disableCursor() {
	INPUT_DEBUG_CHECK;
	glfwSetInputMode(Input::handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(Input::handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::resetCursor() {
	INPUT_DEBUG_CHECK;
	glfwSetInputMode(Input::handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(Input::handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

glm::vec2 Input::getMousePos() {
	double x_pos, y_pos;
	glfwGetCursorPos(Input::handle, &x_pos, &y_pos);
	return glm::vec2(x_pos, y_pos);
}

bool Input::isMouseButtonPressed(int button) {
	return glfwGetMouseButton(Input::handle, button) == GLFW_PRESS;
}
