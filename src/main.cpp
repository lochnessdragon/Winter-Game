#include <filesystem>
#include <iostream>
#include <memory>

#include <camera.h>
#include <log.h>
#include <mesh.h>
#include <object.h>
#include <shader.h>
#include <window.h>
#include <texture.h>

// /nix/store/fif8p12nknb7cw8ffbxbsbrhr2gml3v4-emscripten-3.1.17/share/emscripten/cache/
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

float vertices[] = { -1.0f, -1.0f, 0.0f, 
					  1.0f,  -1.0f, 0.0f,
					  1.0f,  1.0f,  0.0f, 
					  -1.0f, 1.0f,  0.0f };

float uvs[] = { 0.0f, 0.0f, 
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f };

int indices[]{ 0, 1, 2, 0, 2, 3 };

class Application {
private:
	std::shared_ptr<Mesh> quad;
	std::shared_ptr<Object> obj;
	std::shared_ptr<Shader> commonShader;
	std::shared_ptr<Texture> commonTexture;

	GLint modelMatId;
	GLint viewMatId;
	GLint projMatId;
	GLint sampler2DId;

	std::shared_ptr<OrthoCamera> camera;
	double deltaTime;
	double lastTime;
	
	double camSpeed;

public:
	std::shared_ptr<Window> win;

	Application() {
		win = std::make_shared<Window>("Game Engine - v 0.1", 600, 400);
		Log::getGameLog()->trace("Surface created");

		glm::ivec2 winSize = win->getWindowSize();
		Log::getGameLog()->trace("Creating a camera: win_size x={} y={}", winSize.x, winSize.y);
		camera = std::make_shared<OrthoCamera>(win, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), winSize);
		this->camSpeed = 45.0f;

		quad = std::make_shared<Mesh>((GLuint) sizeof(vertices), vertices, (GLuint) sizeof(indices), indices, (GLuint) sizeof(uvs), uvs);
		Log::getGameLog()->trace("Creating object");
		obj = std::make_shared<Object>(glm::vec3(winSize.x / 2.0f, winSize.y / 2.0f, -1.0f), glm::vec3(100.0f), glm::vec3(0.0f));

		Log::getGameLog()->info("Loading shaders");
		commonShader = std::make_shared<Shader>("res/shaders/entity.vert", "res/shaders/fragment.frag");
		modelMatId = commonShader->getUniformLocation("modelMat");
		viewMatId = commonShader->getUniformLocation("viewMat");
		projMatId = commonShader->getUniformLocation("projMat");
		sampler2DId = commonShader->getUniformLocation("texture0");
		commonShader->use();

		Log::getGameLog()->trace("Loading textures");
		commonTexture = std::make_shared<Texture>(std::string("res/textures/stone.png"));

		glClearColor(0.1f, 0.7f, 0.7f, 1.0f);

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

		glm::vec3 camPos = this->camera->getPosition();

		if (glfwGetKey(this->win->getHandle(), GLFW_KEY_A)) {
			camPos.x -= deltaTime * this->camSpeed;
			this->camera->setPosition(camPos);
			Log::getGameLog()->info("Camera Pos: X: {} Y: {} Z: {}", camPos.x, camPos.y, camPos.z);
		}
		if (glfwGetKey(this->win->getHandle(), GLFW_KEY_D)) {
			camPos.x += deltaTime * this->camSpeed;
			this->camera->setPosition(camPos);
			Log::getGameLog()->info("Camera Pos: X: {} Y: {} Z: {}", camPos.x, camPos.y, camPos.z);
		}
		if (glfwGetKey(this->win->getHandle(), GLFW_KEY_W)) {
			camPos.y += deltaTime * this->camSpeed;
			this->camera->setPosition(camPos);
			Log::getGameLog()->info("Camera Pos: X: {} Y: {} Z: {}", camPos.x, camPos.y, camPos.z);
		}
		if (glfwGetKey(this->win->getHandle(), GLFW_KEY_S)) {
			camPos.y -= deltaTime * this->camSpeed;
			this->camera->setPosition(camPos);
			Log::getGameLog()->info("Camera Pos: X: {} Y: {} Z: {}", camPos.x, camPos.y, camPos.z);
		}

		obj->tick();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		commonShader->use();
		commonShader->loadUniform(modelMatId, obj->getModelMat());
		commonShader->loadUniform(viewMatId, camera->getViewMat());
		commonShader->loadUniform(projMatId, camera->getProjMat());
		commonShader->loadUniform(sampler2DId, 0);
		glActiveTexture(GL_TEXTURE0);
		commonTexture->bind();
		quad->bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		quad->unbind();

		win->swap();

		glfwPollEvents();
	}

};

Application* app;

void game_loop() {
	app->update();
}

int main() {
	try {
		Log::init();
		Log::getGameLog()->info("CWD: {}", std::filesystem::current_path().string());
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
}
	catch (std::exception& e) {
		std::cout << "FATAL ERROR: " << e.what() << std::endl;
	}

	return 1;
}