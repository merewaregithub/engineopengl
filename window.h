/*this class initializes glfw and glew and creates a window*/
#pragma once
#include "config.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "image_loader.h"
#include "shader.h"
#include "Text.h"
#include "Button.h"
#include "keyboard.h"
#include "mouse.h"
#include "Camera.h"
#include "Model.h"
#include "Animation.h"

class Window {
private:
	Text *fps_count, *fps_fps, *exit_text;
	Font* font;
	Button* button;
	Model *box, *ground, *suzanne;
	Animation *moving_box, *moving_box_2;
private:
	Camera* camera;
private:
	double tickRate = 1 / 60.0f;
	double tickTime = 0.0f;
	double now = 0.0f;
	long renderCount = 0;
	int tickCount = 0;
public:
	static Config* config;
	static GLFWwindow* window;
private:
	void initGLFW();
	void initGLEW();
	void initCamera();
	void loop();
	void tick();
	void render();
public:
	void run();
	Window(const char* path);
	Window();
	~Window();
	void cleanUp();
};