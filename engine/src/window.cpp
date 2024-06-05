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

void Window::updateSize() {
    glm::ivec2 size = getSize();
    glViewport(0, 0, size.x, size.y);
}

void Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* windowWrapper = (Window*)glfwGetWindowUserPointer(window);
    windowWrapper->input->getKeyEventHandler().dispatch({ key, scancode, action, mods });
}

void Window::glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	Window* windowWrapper = (Window*) glfwGetWindowUserPointer(window);
	windowWrapper->input->getMouseMoveEventHandler().dispatch(glm::vec2(xpos, ypos));
}

void Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* windowWrapper = (Window*)glfwGetWindowUserPointer(window);
    windowWrapper->input->getMouseButtonEventHandler().dispatch({button, action, mods});
}

Window::Window(const std::string title, int width, int height) {
    glfwSetErrorCallback(Window::glfwErrorCallback);

    if(!glfwInit()) {
        Log::getRendererLog()->error("GLFW Initialization failed");
        throw std::runtime_error("GLFW couldn't init!");
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef PLATFORM_MAC
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    this->handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if(!this->handle) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a new window!");
    }

    glfwSetFramebufferSizeCallback(this->handle, Window::glfwResizeCallback);
    glfwSetKeyCallback(this->handle, Window::glfwKeyCallback);
	glfwSetCursorPosCallback(this->handle, Window::glfwMouseMoveCallback);
    glfwSetMouseButtonCallback(this->handle, Window::glfwMouseButtonCallback);

    glfwSetWindowUserPointer(this->handle, this);

    glfwMakeContextCurrent(this->handle);

	// enable vsync
	glfwSwapInterval(1);

	#ifndef PLATFORM_WEB
    if(WINDOW_COUNT < 1) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Log::getRendererLog()->error("Failed to initialize GLAD");
            throw std::runtime_error("Couldn't initialize opengl functions.");
        }
    }
	#endif

    input = std::make_shared<Input>(this->handle);

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

void Window::setShouldClose() {
    glfwSetWindowShouldClose(handle, GLFW_TRUE);
}

void Window::swap() {
    glfwSwapBuffers(this->handle);
}

glm::ivec2 Window::getSize() {
    glm::ivec2 size;
    glfwGetWindowSize(this->handle, &size.x, &size.y);
    return size;
}
