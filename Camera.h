#pragma once
#include "Line3D.h"
#include "Triangle3D.h"
#include <iostream>
#include "mouse.h"
#include "keyboard.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
	static GLfloat lastX;
	static GLfloat lastY;
	static Line3D* mouse_sight;
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

	//mouse picker
	glm::vec3 mouse_ray_start, mouse_ray_end;
	glm::vec3 mouse_ray;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f);
public:
	Camera::Camera(float x, float y, float z);
	void Camera::init(int w, int h);
	void Camera::update();
	Camera::~Camera();
};
