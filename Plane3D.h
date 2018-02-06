#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "config.h"

class Plane3D {
public:
	glm::vec3 *p1, *p2, *p3, *p4;
	glm::vec3 normal;
public:
	Shader* shader;
	unsigned int VBO, VAO;
public:
	Plane3D(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
		this->p1 = new glm::vec3(p1.x, p1.y, p1.z);
		this->p2 = new glm::vec3(p2.x, p2.y, p2.z);
		this->p3 = new glm::vec3(p3.x, p3.y, p3.z);
		this->p4 = new glm::vec3(p4.x, p4.y, p4.z);

		normal = glm::vec3(0.0f, 0.0f, 1.0f);
		initGL();
	}

	void Plane3D::initGL() {
		shader = new Shader("shape3d.vs", "shape3d.fs", nullptr);
		shader->use();
		shader->setMat4("projection", glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f));

		float vertices[] = {
			p1->x, p1->y, p1->z,
			p2->x, p2->y, p2->z,
			p3->x, p3->y, p3->z,
			p4->x, p4->y, p4->z
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Plane3D::render(glm::mat4 view, glm::vec3 translate) {
		shader->use();
		shader->setMat4("view", view);
		glm::mat4 model(1);
		model = glm::translate(model, translate);
		shader->setMat4("model", model);
		glPolygonMode(GL_BACK, GL_LINE);
		//glLineWidth(2.0f);
		glDisable(GL_CULL_FACE);
		glBindVertexArray(VAO);
		glDrawArrays(GL_QUADS, 0, 4);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glLineWidth(1.0f);
	}

	void Plane3D::cleanUp() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	Plane3D() {

	}

	~Plane3D() {

	}
};