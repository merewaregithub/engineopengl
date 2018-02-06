#pragma once
#include <iostream>
#include "mouse.h"
#include "keyboard.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	static GLfloat lastX;
	static GLfloat lastY;
public:
	glm::mat4 view;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat currentFrame = 0.0f;

	GLfloat xoffset = 0.0f;
	GLfloat yoffset = 0.0f;

	GLfloat sensitivity = 0.08;
	GLfloat cameraSpeed = 1.0f;

	glm::vec3 front, true_up, true_side;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
public:
	Camera::Camera(float x, float y, float z);
	void Camera::init(int w, int h);
	void Camera::update();
	Camera::~Camera();
};
