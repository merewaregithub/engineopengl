#pragma once
#include "GLFW/glfw3.h"
#include "global.h"
#include <iostream>

#define MOUSE_BUTTON_COUNT 32

class Mouse {
public:
	static bool old_actions[MOUSE_BUTTON_COUNT];
	static bool new_actions[MOUSE_BUTTON_COUNT];
	static float mouse_x, mouse_y, mouse_gl_x, mouse_gl_y;
	static bool enabled;
public:
	static void setPosition(GLFWwindow* window, double xpos, double ypos);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static float Mouse::width_to_gl();
	static float Mouse::height_to_gl();
	Mouse();
	~Mouse();

};

