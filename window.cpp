#include "window.h"

GLFWwindow* Window::window;
Config* Window::config;

Window::Window(const char* path) {
	this->config = new Config(path);
}

Window::Window() {

}

static void exit() {
	glfwSetWindowShouldClose(Window::window, GLFW_TRUE);
}

static void checkKeys() {
	if (Keyboard::keys[GLFW_KEY_ESCAPE]) {
		if (Mouse::enabled) {
			glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Mouse::enabled = false;
		}
		else {
			glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Mouse::enabled = true;
		}
		Mouse::setPosition(Window::window, Window::config->WIDTH / 2, Window::config->HEIGHT / 2);
		Camera::lastX = Window::config->WIDTH / 2;
		Camera::lastY = Window::config->HEIGHT / 2;
		Mouse::mouse_gl_x = 0.0f;
		Mouse::mouse_gl_y = 0.0f;
	}
} 

void Window::run() {
	initGLFW();
	initGLEW();
	initCamera();
	font = new Font("calibri");
	fps_count = new Text("", font, 0.3f);
	fps_fps = new Text("FPS", font, 0.3f);
	exit_text = new Text("EXIT", font, 0.5f);
	fps_count->setColor(200 / 255.0f, 205 / 255.0f, 215 / 255.0f);
	fps_fps->setColor(203 / 255.0f, 209 / 255.0f, 219 / 255.0f);
	exit_text->setColor(255 / 255.0f, 100 / 255.0f, 100 / 255.0f);
	button = new Button(exit_text, 0.0f, 0.0f, 0.03f, 0.02f, exit);
	button->setColor(0.5f, 0.2f, 0.2f);
	box = new Model("cubes");
	ground = new Model("ground");
	//suzanne = new Model("collide");
	suzanne2 = new Model("deskobj");
	//moving_box = new Animation("moving_box", 60);

	collision = new Collision();
	//collision->add(suzanne);
	//collision->add(suzanne2);
	collide_anim = new Animation("collide_anim", 60);
	collision->add(collide_anim);

	Shader* lightingShader = new Shader("multilight.vs", "multilight.fs");
	lightingShader->use();
	lightingShader->setMat4("projection", glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f));
	lightingShader->setInt("material.diffuse", 0);
	//spotlightShader.setInt("material.specular", 1);
	lightingShader->setFloat("material.shininess", 32.0f);

	lightingShader->setInt("point_light_count", 1);
	lightingShader->setInt("spot_light_count", 0);

	dirLight0 = new DirectLight(lightingShader, 0, glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	pointLight0 = new PointLight(lightingShader, 0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0009f, 0.032f);

	ground->setShader(lightingShader);
	suzanne2->setShader(lightingShader);

	loop();
}

void Window::tick() {
	camera->update();

	ground->move(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//suzanne->move(glm::vec3(glfwGetTime()*2, glfwGetTime()*2, glfwGetTime()*2), glm::vec3(0.0f, 2.0f, 0.0f));
	suzanne2->move(glm::vec3(glfwGetTime() * -2, glfwGetTime() * 2, glfwGetTime() * -3), glm::vec3(3.0f, 2.0f, 0.0f));
	button->tick();
	//moving_box->tick();
	//moving_box->move(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	collide_anim->tick();
	collide_anim->move(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	pointLight0->setPosition(glm::vec3(0.0f, glfwGetTime()/20.0f, 0.0f));

	collision->checkMouseModelCollision();
}

void Window::render() {
	//render 3d
	glEnable(GL_DEPTH_TEST);
	//Camera::mouse_sight->render(camera->view, glm::vec3(0.0f, 0.0f, 0.0f));
	//box->render(camera->view, glm::vec3(0.0f, 2.0f, 0.0));
	ground->render(camera->view);
	//suzanne->render(camera->view);
	//suzanne->bounds->render(camera->view);
	suzanne2->render(camera->view);
	//suzanne2->bounds->render(camera->view);
	//moving_box->render(camera->view);
	//collide_anim->render(camera->view);

	pointLight0->show(camera->view);

	//render gui
	glDisable(GL_DEPTH_TEST);
	fps_count->render(glm::vec3(2.0f - fps_fps->gl_width*0.3f - fps_count->gl_width*0.3f - 0.02f, 0.01f, 0.0f));
	fps_fps->render(glm::vec3(2.0f - fps_fps->gl_width*0.3f - 0.01f, 0.01f, 0.0f));
	button->render();
}

void Window::loop() {
	while (!glfwWindowShouldClose(this->window)) {
		glfwPollEvents();
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);

		frameBuffer->bind();

		if (glfwGetTime() - tickTime >= tickRate) {
			tickTime = glfwGetTime();
			tick();
			tickCount++;
		}

		render();
		frameBuffer->render();
		frameBuffer->unbind();
		renderCount++;

		if (glfwGetTime() - now >= 1.0f) {
			now = glfwGetTime();
			fps_count->setText(std::to_string(renderCount));
			//std::cout << "FPS: " << renderCount << " UPS: " << tickCount << std::endl;
			renderCount = 0;
			tickCount = 0;
		}

		glfwSwapBuffers(this->window);
	}
}

void Window::initCamera() {
	camera = new Camera(0.0f, 0.0f, 0.0f);
	camera->init(config->WIDTH / 2, config->HEIGHT / 2);
	Keyboard::setWindowCallback(checkKeys);
}

void Window::initGLFW() {
	if(glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("failed to initialize glfw!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, config->DECORATED);
	glfwWindowHint(GLFW_RESIZABLE, config->RESIZEABLE); // TODO: set resize callback

	config->FULLSCREEN ? this->window = glfwCreateWindow(config->WIDTH, config->HEIGHT, "Mereware", glfwGetPrimaryMonitor(), nullptr) : this->window = glfwCreateWindow(config->WIDTH, config->HEIGHT, "Mereware", nullptr, nullptr);
	const GLFWvidmode* primaryMonitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(this->window, primaryMonitor->width / 2 - config->WIDTH / 2, primaryMonitor->height / 2 - config->HEIGHT / 2);
	glfwSetCursorPos(window, config->WIDTH / 2, config->HEIGHT / 2);
	Mouse::setPosition(window, config->WIDTH / 2, config->HEIGHT / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(this->window);

	glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
	glfwSetCursorPosCallback(window, Mouse::cursor_pos_callback);
	glfwSetKeyCallback(window, Keyboard::key_callback);
}

void Window::initGLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("failed to initialize glew!");
	}



	glViewport(0, 0, config->WIDTH, config->HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSwapInterval(0);

	frameBuffer = new FrameBuffer();
}


void Window::cleanUp() {
	delete config;
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

Window::~Window() {

}