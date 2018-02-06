#include "mouse.h"


bool Mouse::new_actions[MOUSE_BUTTON_COUNT];
bool Mouse::old_actions[MOUSE_BUTTON_COUNT];
float Mouse::mouse_x;
float Mouse::mouse_y;
float Mouse::mouse_gl_x;
float Mouse::mouse_gl_y;
bool Mouse::enabled = false;

Mouse::Mouse() {

}

void Mouse::setPosition(GLFWwindow * window, double xpos, double ypos) {
	Mouse::mouse_x = xpos;
	Mouse::mouse_y = ypos;
	glfwSetCursorPos(window, xpos, ypos);
}

void Mouse::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	Mouse::mouse_x = xpos;
	Mouse::mouse_y = ypos;
	Mouse::mouse_gl_x = width_to_gl();
	Mouse::mouse_gl_y = height_to_gl();

	old_actions[GLFW_MOUSE_BUTTON_LEFT] = new_actions[GLFW_MOUSE_BUTTON_LEFT];
	//std::cout << "x: " << width_to_gl() << " y: " << height_to_gl() << std::endl;
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button >= 0 && button < MOUSE_BUTTON_COUNT) {
		old_actions[button] = new_actions[button];
		new_actions[button] = action;
	}
	//std::cout << old_actions[button] << " " << new_actions[button] << std::endl;
}

float Mouse::width_to_gl() {
	return (Mouse::mouse_x) * (2.0f) / (app_width - 0.0f) + (-1.0f);
}

float Mouse::height_to_gl() {
	return -1.0f*((Mouse::mouse_y) * (2.0f) / (app_height - 0.0f) + (-1.0f));
}

Mouse::~Mouse() {

}
