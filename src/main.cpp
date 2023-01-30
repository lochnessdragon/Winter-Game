#include <filesystem>
#include <iostream>
#include <memory>

#include <camera.h>
#include <log.h>
#include <mesh.h>
#include <object.h>
#include <shader.h>
#include <window.h>

// /nix/store/fif8p12nknb7cw8ffbxbsbrhr2gml3v4-emscripten-3.1.17/share/emscripten/cache/
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

float vertices[] = {-1.0f, -1.0f, 0.0f, 1.0f,  -1.0f, 0.0f,
                    1.0f,  1.0f,  0.0f, -1.0f, 1.0f,  0.0f};

int indices[]{0, 1, 2, 0, 2, 3};

class Application {
private:
  std::shared_ptr<Mesh> quad;
  std::shared_ptr<Object> obj;
  std::shared_ptr<Shader> commonShader;
  GLint modelMatId;
  GLint viewMatId;
  GLint projMatId;

  std::shared_ptr<PerspectiveCamera> camera;
  double deltaTime;
  double lastTime;

  static Application* SINGLETON;

public:
  std::shared_ptr<Window> win;
  
  Application() {
	SINGLETON = this;
    win = std::make_shared<Window>("Game Engine - v 0.1", 600, 400);
	Log::getGameLog()->trace("Surface created");

    quad = std::make_shared<Mesh>(sizeof(vertices), vertices, sizeof(indices), indices);
	Log::getGameLog()->trace("Creating object");
    obj = std::make_shared<Object>(glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(1.0f), glm::vec3(0.0f), quad);

	Log::getGameLog()->info("Loading shaders");
    commonShader = std::make_shared<Shader>("res/shaders/vertex.vert", "res/shaders/fragment.frag");
    modelMatId = commonShader->getUniformLocation("modelMat");
    viewMatId = commonShader->getUniformLocation("viewMat");
    projMatId = commonShader->getUniformLocation("projMat");
    commonShader->use();

	Log::getGameLog()->trace("Creating a camera");
    glm::ivec2 winSize = win->getWindowSize();
    camera = std::make_shared<PerspectiveCamera>(win, glm::vec3(4.0f, 0.0f, 10.0f), glm::vec3(0.0f),
           (float)winSize.x / (float)winSize.y, glm::radians(120.0f), 0.1f,
           1000.0f);

    glClearColor(0.1f, 0.7f, 0.7f, 1.0f);

    deltaTime = 0.0f;
    lastTime = glfwGetTime();

	Log::getGameLog()->trace("Initialization finished");
  }

  void update() {
    // tick
	Log::getGameLog()->trace("Tick");
    {
      double now = glfwGetTime();
      deltaTime = now - lastTime;
      lastTime = now;
    }

	obj->tick();

    // render
	Log::getRendererLog()->trace("Render");
    glClear(GL_COLOR_BUFFER_BIT);

	Log::getRendererLog()->trace("Setting up the shader");
    commonShader->use();
	Log::getRendererLog()->trace("Uploading model matrix");
    commonShader->loadUniform(modelMatId, obj->getModelMat());
	Log::getRendererLog()->trace("Uploading view matrix");
    commonShader->loadUniform(viewMatId, camera->getViewMat());
	Log::getRendererLog()->trace("Uploading projection matrix");
    commonShader->loadUniform(projMatId, camera->getProjMat());

	Log::getRendererLog()->trace("Binding the quad");
    quad->bind();
	
	Log::getRendererLog()->trace("Drawing the quad");
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	Log::getRendererLog()->trace("Unbinding the quad");
    quad->unbind();

	Log::getRendererLog()->trace("Swapping surfaces");
    win->swap();

    glfwPollEvents();
  }

  static Application* instance() { return Application::SINGLETON; };
};

Application* Application::SINGLETON = nullptr;

void game_loop() {
	Application::instance()->update();
}
	  
int main() {
  try {
    Log::init();
    Log::getGameLog()->info("CWD: {}", std::filesystem::current_path().string());
    Log::getGameLog()->info("Game startup...");
    Application app;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(game_loop, 0, false);
#else
    while (!app.win->shouldClose()) {
      app.update()
    }
#endif

        return 0;
  } catch (std::exception &e) {
    std::cout << "FATAL ERROR: " << e.what() << std::endl;
  }

  return 1;
}