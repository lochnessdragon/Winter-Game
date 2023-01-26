#include <iostream>
#include <stdexcept>

#include "window.h"
#include "log.h"

uint32_t Window::WINDOW_COUNT = 0;

void Window::glfwErrorCallback(int e, const char *d) {
    Log::getRendererLog()->error("GLFW Error#{}: {}", e, d);
}

void Window::glfwResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Window* windowWrapper = (Window*) glfwGetWindowUserPointer(window);
    windowWrapper->windowResizeEvent.dispatch({ width, height });
}

void Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* windowWrapper = (Window*)glfwGetWindowUserPointer(window);
    windowWrapper->keyPressedEvent.dispatch({key, scancode, action, mods});
}

Window::Window(const std::string& title, int width, int height) {
    glfwSetErrorCallback(Window::glfwErrorCallback);

    if(!glfwInit()) {
        Log::getRendererLog()->error("GLFW Initialization failed");
        throw std::runtime_error("GLFW couldn't init!");
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    this->handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if(!this->handle) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a new window!");
    }

    glfwSetFramebufferSizeCallback(this->handle, Window::glfwResizeCallback);
    glfwSetKeyCallback(this->handle, Window::glfwKeyCallback);

    glfwSetWindowUserPointer(this->handle, this);

    glfwMakeContextCurrent(this->handle);

    if(WINDOW_COUNT < 1) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Log::getRendererLog()->error("Failed to initialize GLAD");
            throw std::runtime_error("Couldn't initialize opengl functions.");
        }
    }

    WINDOW_COUNT++;
}

Window::~Window() {
    glfwDestroyWindow(handle);
    WINDOW_COUNT--;

    if(WINDOW_COUNT < 1) {
        glfwTerminate();
    }
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(handle);
}

void Window::swap() {
    glfwSwapBuffers(this->handle);
}

glm::ivec2 Window::getWindowSize() {
    glm::ivec2 size;
    glfwGetWindowSize(this->handle, &size.x, &size.y);
    return size;
}