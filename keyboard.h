#pragma once
#include "GLFW/glfw3.h"

#define KEYBOAR_KEY_COUNT 1024

class Keyboard {
public:
	static bool keys[KEYBOAR_KEY_COUNT];
	static void(*window_callback)();
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void setWindowCallback(void(*ptr)());
	Keyboard();
	~Keyboard();

};

