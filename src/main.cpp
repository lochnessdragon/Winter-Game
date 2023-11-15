//#include <filesystem>
#include <iostream>
#include <memory>

// engine files
#include <camera.h>
#include <log.h>
#include <mesh.h>
#include <object.h>
#include <shader.h>
#include <window.h>
#include <texture.h>
#include <renderer.h>
#include <mesh_generator.h>
#include <input.h>
#include <ui/text-renderer.h>

// project files
#include "camera_controller.h"

// /nix/store/fif8p12nknb7cw8ffbxbsbrhr2gml3v4-emscripten-3.1.17/share/emscripten/cache/
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

/*
* Features to implements before Github Game Off 2023
* 1. 2D rendering
* 2. Text rendering
* 3. Scene system
* 4. Audio system
* 5. ImGUI
* 6. ECS
*/

class Application {
private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Object> obj;
	std::shared_ptr<Shader> commonShader;
	std::shared_ptr<Texture> commonTexture;

	std::shared_ptr<PerspectiveCamera> camera;
	CameraController cameraController;

	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<Font> font;
    
    AmbientLight ambientLight;
    Light basicLight;

	double deltaTime;
	double lastTime;

public:
	std::shared_ptr<Window> win;

	Application() {
		win = std::make_shared<Window>("Horo geam", 1200, 800);
		Log::getGameLog()->trace("Surface created");
        win->getKeyEventHandler().addListener([this](auto keyData){
            if(keyData.key == GLFW_KEY_Z && keyData.action == GLFW_RELEASE) {
                this->renderer->toggleWireframe();
			} else if (keyData.key == GLFW_KEY_ESCAPE && keyData.action == GLFW_PRESS) {
				win->setShouldClose();
			}
        });
		
		// set up input system
		Input::setHandle(win->getHandle());
		Input::disableCursor();

		renderer = std::make_shared<Renderer>();
		textRenderer = std::make_shared<TextRenderer>(win);

		// create a font
		font = std::make_shared<Font>("res/fonts/OpenSans-VariableFont_wdth,wght.ttf");

		glm::ivec2 winSize = win->getWindowSize();
		Log::getGameLog()->trace("Creating a camera: win_size x={} y={}", winSize.x, winSize.y);
		camera = std::make_shared<PerspectiveCamera>(win, glm::vec3(0.0f, 8.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f), (float) winSize.x / (float) winSize.y, glm::radians(70.0f), 0.1f, 1000.0f);
		cameraController = CameraController(camera);

        mesh = loadObj("res/models/house.obj");
		Log::getGameLog()->trace("Creating object");
		obj = std::make_shared<Object>(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));

		Log::getGameLog()->info("Loading shaders");
		commonShader = std::make_shared<Shader>("res/shaders/entity.vert", "res/shaders/fragment.frag");
		commonShader->use();

		Log::getGameLog()->trace("Loading textures");
        unsigned char whiteTex[] = { 255, 255, 255, 255};
		TextureSpecification spec;
		spec.width = 1;
		spec.height = 1;
		commonTexture = std::make_shared<Texture>(spec, whiteTex);
        
        ambientLight = { glm::vec3(0.1f, 0.7f, 0.7f), 0.2f };
        basicLight = { glm::vec3(12.0f, 12.0f, 15.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.75f };

		renderer->setClearColor(0.1f, 0.7f, 0.7f, 1.0f);

		deltaTime = 0.0f;
		lastTime = glfwGetTime();
		
		Log::getGameLog()->trace("Initialization finished");
	}

	void update() {
		// tick
		{
			double now = glfwGetTime();
			deltaTime = now - lastTime;
			lastTime = now;
		}

		cameraController.update(deltaTime);

		// render
		renderer->clear();
		renderer->render(camera, obj, mesh, commonShader, commonTexture, ambientLight, basicLight);
		textRenderer->render("Some debug text", font, { 10.0, 72.0 }, 72, { 1.0f, 1.0f, 1.0f, 1.0f });
		textRenderer->endFrame();
		win->swap();

		Input::update();
	}

};

Application* app;

void game_loop() {
	app->update();
}

int main() {
	try {
		Log::init();
		//Log::getGameLog()->info("CWD: {}", std::filesystem::current_path().string());
		Log::getGameLog()->info("Game startup...");
		app = new Application();

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(game_loop, 0, false);
#else
		while (!app->win->shouldClose()) {
			app->update();
		}
#endif

		return 0;
    } catch (std::exception& e) {
		std::cout << "FATAL ERROR: " << e.what() << std::endl;
	}

	return 1;
}
