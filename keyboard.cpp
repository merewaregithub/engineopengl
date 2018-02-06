#include "keyboard.h"

bool Keyboard::keys[KEYBOAR_KEY_COUNT];
void(*Keyboard::window_callback)();

Keyboard::Keyboard() {
	for (int i = 0; i < KEYBOAR_KEY_COUNT; i++) {
		this->keys[i] = false;
	}
}

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key >= 0 && key < KEYBOAR_KEY_COUNT) {
		keys[key] = action != GLFW_RELEASE;
		window_callback();
	}
	
}

void Keyboard::setWindowCallback(void(*ptr)()) {
	Keyboard::window_callback = ptr;
}

Keyboard::~Keyboard() {

}
