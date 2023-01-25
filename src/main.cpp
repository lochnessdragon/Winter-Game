#include <iostream>
#include <filesystem>

#include <log.h>
#include <window.h>
#include <shader.h>
#include <object.h>
#include <mesh.h>
#include <camera.h>

float vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f
};

int indices[] {
    0, 1, 2,
    0, 2, 3
};

int main() {
    try {
        Log("Opening Window!");
        std::cout << "CWD: " << std::filesystem::current_path() << std::endl;

        Window win("Minecraft - v 0.1", 600, 400);

        Mesh quad(sizeof(vertices), vertices, sizeof(indices), indices);
        Object obj(glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(1.0f), glm::vec3(0.0f), quad);

        Shader commonShader("res/shaders/vertex.vsh", "res/shaders/fragment.fsh");
        GLint modelMatId = commonShader.getUniformLocation("modelMat");
        GLint viewMatId = commonShader.getUniformLocation("viewMat");
        GLint projMatId = commonShader.getUniformLocation("projMat");
        commonShader.use();

        glm::ivec2 winSize = win.getWindowSize();
        Camera camera(&win, glm::vec3(4.0f, 0.0f, 10.0f), glm::vec3(0.0f), (float) winSize.x / (float) winSize.y, glm::radians(120.0f), 0.1f, 1000.0f);

        glClearColor(0.1f, 0.7f, 0.7f, 1.0f);

        double deltaTime = 0.0f;
        double lastTime = glfwGetTime();

        while (!win.shouldClose()) {

            // tick
            {
                double now = glfwGetTime();
                deltaTime = now - lastTime;
                lastTime = now;
            }

            // render
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
    catch (std::exception& e) {
        std::cout << "FATAL ERROR: " << e.what() << std::endl;
    }

    return 1;
}