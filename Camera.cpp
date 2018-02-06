#include "Camera.h"

GLfloat Camera::lastX = 0;
GLfloat Camera::lastY = 0;

Camera::Camera(float x, float y, float z) {
	cameraPos.x = x;
	cameraPos.y = y;
	cameraPos.z = z;

}

void Camera::init(int w, int h) {
	lastX = w / 2;
	lastY = h / 2;
}

void Camera::update() {
	if (!Mouse::enabled) {
		xoffset = Mouse::mouse_x - lastX;
		yoffset = lastY - Mouse::mouse_y;

		lastX = Mouse::mouse_x;
		lastY = Mouse::mouse_y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw -= xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(-yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	true_side = glm::normalize(glm::cross(cameraFront, cameraUp));
	true_up = glm::normalize(glm::cross(cameraFront, true_side));

	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	cameraSpeed = 5.0f * deltaTime;
	if (Keyboard::keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (Keyboard::keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (Keyboard::keys[GLFW_KEY_A])
		cameraPos -= true_side * cameraSpeed;
	if (Keyboard::keys[GLFW_KEY_D])
		cameraPos += true_side * cameraSpeed;

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	
}

Camera::~Camera() {

}
