#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

class Window {
private:
    GLFWwindow* handle;

    static uint32_t WINDOW_COUNT;

    static void glfwErrorCallback(int e, const char *d);
    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool shouldClose();
    void swap();
    glm::ivec2 getWindowSize();
};