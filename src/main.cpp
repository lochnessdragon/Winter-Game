#include <iostream>

#include <log.h>
#include <window.h>
#include <shader.h>
#include <object.h>
#include <mesh.h>
#include <camera.h>

float vertices[] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

int indices[] {
    0, 1, 2,
    0, 2, 3
};

int main() {
    Log("Opening Window!");

    Window win("Minecraft - v 0.1", 400, 200);

    Mesh quad(sizeof(vertices), vertices, sizeof(indices), indices);
    Object obj(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), quad);

    Shader commonShader("res/shaders/vertex.vsh", "res/shaders/fragment.fsh");
    GLint modelMatId = commonShader.getUniformLocation("modelMat");
    GLint viewMatId = commonShader.getUniformLocation("viewMat");
    GLint projMatId = commonShader.getUniformLocation("projMat");
    commonShader.use();

    glm::ivec2 winSize = win.getWindowSize();
    Camera camera(glm::vec3(-4.0f, -4.0f, 0.0f), glm::vec3(0.0f), winSize.x/winSize.y, glm::radians(120.0f), 0.1f, 1000.0f);

    glClearColor(0.1f, 0.7f, 0.7f, 1.0f);

    while(!win.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        obj.tick();

        commonShader.use();
        commonShader.loadUniform(modelMatId, obj.getModelMat());
        commonShader.loadUniform(viewMatId, camera.getViewMat());
        commonShader.loadUniform(projMatId, camera.getProjMat());

        quad.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        quad.unbind();

        win.swap();

        glfwPollEvents();
    }

    return 0;
}