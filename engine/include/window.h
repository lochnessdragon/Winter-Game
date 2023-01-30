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

    EventDispatcher<WindowResizeEventData> windowResizeEvent;
    EventDispatcher<KeyEventData> keyPressedEvent;
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool shouldClose();
    void swap();
    glm::ivec2 getWindowSize();

    auto& getWindowResizeHandler() { return windowResizeEvent; };
    auto& getKeyPressedHandler() { return keyPressedEvent; };
};