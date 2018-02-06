#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "config.h"
#include "Line3D.h"

class Triangle3D {
public:
	glm::vec3 translate, rotate;
	glm::mat4 projection, model;
private:
	glm::vec3 *A, *B, *C;
	glm::vec3 *normal;
public:
	Shader* shader;
	unsigned int VBO, VAO;
	int polyMode = GL_LINE;
public:
	Triangle3D(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal) {
		this->A = new glm::vec3(p1.x, p1.y, p1.z);
		this->B = new glm::vec3(p2.x, p2.y, p2.z);
		this->C = new glm::vec3(p3.x, p3.y, p3.z);
		this->normal = new glm::vec3(normal.x, normal.y, normal.z);
		projection = glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f);
		initGL();
	}

	void Triangle3D::initGL() {
		shader = new Shader("shape3d.vs", "shape3d.fs", nullptr);
		shader->use();
		shader->setMat4("projection", projection);

		float vertices[] = {
			A->x, A->y, A->z,
			B->x, B->y, B->z,
			C->x, C->y, C->z
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

	glm::vec3 Triangle3D::getA() {
		glm::vec4 result = model * glm::vec4((*A), 1.0f);
		return glm::vec3(result.x, result.y, result.z);
	}

	glm::vec3 Triangle3D::getB() {
		glm::vec4 result = model * glm::vec4((*B), 1.0f);
		return glm::vec3(result.x, result.y, result.z);
	}

	glm::vec3 Triangle3D::getC() {
		glm::vec4 result = model * glm::vec4((*C), 1.0f);
		return glm::vec3(result.x, result.y, result.z);
	}

	glm::vec3 Triangle3D::getNormal() {
		glm::vec4 result = model * glm::vec4((*normal), 1.0f);
		return glm::vec3(result.x, result.y, result.z)-translate;
	}

	void Triangle3D::move(glm::vec3 rotate, glm::vec3 translate) {
		this->rotate = rotate;
		this->translate = translate;
		glm::mat4 modelMatrix(1);
		modelMatrix = glm::translate(modelMatrix, translate);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = modelMatrix;
	}

	void Triangle3D::render(glm::mat4 view) {
		shader->use();
		shader->setMat4("view", view);
		shader->setMat4("model", model);
		glPolygonMode(GL_FRONT_AND_BACK, polyMode);
		//glLineWidth(2.0f);
		glDisable(GL_CULL_FACE);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glLineWidth(1.0f);
	}

	void Triangle3D::cleanUp() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	Triangle3D() {

	}

	~Triangle3D() {

	}
};