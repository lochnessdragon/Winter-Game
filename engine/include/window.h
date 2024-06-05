#pragma once

// System files
#include <string>
#include <memory>

// necessary for proper includes
#include "platform.h"

// Engine dependencies
#include "GLAPI.h"

#include <glm/glm.hpp>

// Engine files
#include "event.h"
#include "surface.h"
#include "input.h"

struct WindowResizeEventData {
    int width;
    int height;
};

class Window : public Surface {
private:
    GLFWwindow* handle;

    static uint32_t WINDOW_COUNT;

    static void glfwErrorCallback(int e, const char *d);
    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
    EventDispatcher<WindowResizeEventData> windowResizeEvent;
    // DEPRECATED - moved to input class
 //   EventDispatcher<KeyEventData> keyEvent;
	//EventDispatcher<glm::vec2> mouseMoveEvent;
 //   EventDispatcher<MouseButtonEventData> mouseButtonEvent;
    std::shared_ptr<Input> input;
public:
    Window(const std::string title, int width, int height);
    ~Window();

    bool shouldClose();
    void setShouldClose();
    
    void swap();
    glm::ivec2 getSize();
    void updateSize();

    GLFWwindow* getHandle() { return handle; };

    auto& getWindowResizeHandler() { return windowResizeEvent; };

    // See above deprecated warning
    //   auto& getKeyEventHandler() { return keyEvent; };
	//auto& getMouseMoveEventHandler() { return mouseMoveEvent; };
 //   auto& getMouseButtonEventHandler() { return mouseButtonEvent; };
};
