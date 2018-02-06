#include "Camera.h"

GLfloat Camera::lastX = 0;
GLfloat Camera::lastY = 0;
Line3D* Camera::mouse_sight;

Camera::Camera(float x, float y, float z) {
	cameraPos.x = x;
	cameraPos.y = y;
	cameraPos.z = z;
	mouse_sight = new Line3D(glm::vec3(0.0f), glm::vec3(0.0f));
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

	glm::vec4 clipCoord(Mouse::mouse_gl_x, Mouse::mouse_gl_y, -1.0f, 1.0f);

	glm::mat4 inverted_projection = glm::inverse(projection);
	
	glm::vec4 eye_temp = inverted_projection * clipCoord;
	glm::vec4 eyeCoord = glm::vec4(eye_temp.x, eye_temp.y, -1.0f, 0.0f);

	glm::mat4 inverted_view = glm::inverse(view);
	glm::vec4 ray_world = inverted_view * eyeCoord;

	mouse_ray = glm::normalize(glm::vec3(ray_world.x , ray_world.y, ray_world.z));

	//if (Keyboard::keys[GLFW_KEY_SPACE]) {

	mouse_ray_start = cameraPos;
	mouse_ray_end = cameraPos + 20.0f * mouse_ray;
	//mouse_sight->set(mouse_ray_start, mouse_ray_end);
	delete mouse_sight;
	mouse_sight = new Line3D(mouse_ray_start, mouse_ray_end);
	//}
}

Camera::~Camera() {

}
