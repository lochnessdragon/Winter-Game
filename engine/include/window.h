#pragma once

#include "platform.h"

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "event.h"

struct WindowResizeEventData {
    int width;
    int height;
};

struct KeyEventData {
    int key;
    int scancode;
    int action;
    int mods;
};

class Window {
private:
    GLFWwindow* handle;

    static uint32_t WINDOW_COUNT;

    static void glfwErrorCallback(int e, const char *d);
    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	
    EventDispatcher<WindowResizeEventData> windowResizeEvent;
    EventDispatcher<KeyEventData> keyEvent;
	EventDispatcher<glm::vec2> mouseMoveEvent;
public:
    Window(const std::string title, int width, int height);
    ~Window();

    bool shouldClose();
    void setShouldClose();
    
    void swap();
    glm::ivec2 getWindowSize();

    GLFWwindow* getHandle() { return handle; };

    auto& getWindowResizeHandler() { return windowResizeEvent; };
    auto& getKeyEventHandler() { return keyEvent; };
	auto& getMouseMoveEventHandler() { return mouseMoveEvent; };
};
