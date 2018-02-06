#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "config.h"
#include "Triangle3D.h"
#include <vector>

struct TrianglePointPair{
public:
	Triangle3D* triangle;
	glm::vec3 P;
public:
	TrianglePointPair(Triangle3D* tri, glm::vec3 point) {
		this->triangle = tri;
		this->P = point;
	}
	TrianglePointPair() {

	}
};

class Line3D {
public:
	glm::vec3* start, *end;
public:
	Shader* shader;
	GLuint VBO, VAO;
public:
	Line3D(glm::vec3 start, glm::vec3 end) {
		this->start = new glm::vec3(start.x, start.y, start.z);
		this->end = new glm::vec3(end.x, end.y, end.z);
		initGL();
	}

	void Line3D::initGL() {
		shader = new Shader("shape3d.vs", "shape3d.fs", nullptr);
		shader->use();
		shader->setMat4("projection", glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f));

		float vertices[] = {
			start->x, start->y, start->z,
			end->x, end->y, end->z
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

	void Line3D::render(glm::mat4 view, glm::vec3 translate) {
		shader->use();
		shader->setMat4("view", view);
		glm::mat4 model(1);
		model = glm::translate(model, translate);
		shader->setMat4("model", model);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glLineWidth(2.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		//glPolygonMode(GL_FRONT, GL_FILL);
		//glLineWidth(1.0f);
	}

	void Line3D::set(glm::vec3 a, glm::vec3 b) {
		this->start->x = a.x;
		this->start->y = a.y;
		this->start->z = a.z;
		this->end->x = b.x;
		this->end->y = b.y;
		this->end->z = b.z;
	}

	void Line3D::cleanUp() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	TrianglePointPair Line3D::intersects(Triangle3D *triangle) {
		glm::vec3 v = *end - *start;
		glm::vec3 w = triangle->getA() - *start;

		float k = glm::dot(w, triangle->getNormal()) / glm::dot(v, triangle->getNormal());

		glm::vec3 P = *start + k * v;

		if (k >= 0 && k <= 1) {
			glm::vec3 uu = triangle->getB() - triangle->getA();
			glm::vec3 vv = triangle->getC() - triangle->getA();
			glm::vec3 ww = P - triangle->getA();

			glm::vec3 vCw = glm::cross(vv, ww);
			glm::vec3 vCu = glm::cross(vv, uu);

			if (glm::dot(vCw, vCu) < 0) {
				return TrianglePointPair(triangle, *start);
			}

			glm::vec3 uCw = glm::cross(uu, ww);
			glm::vec3 uCv = glm::cross(uu, vv);

			if (glm::dot(uCw, uCv) < 0) {
				return TrianglePointPair(triangle, *start);
			}

			float denom = glm::length(uCv);
			float r = glm::length(vCw) / denom;
			float t = glm::length(uCw) / denom;

			if (r + t <= 1) {
				return TrianglePointPair(triangle, P);
			}
			else {
				return TrianglePointPair(triangle, *start);
			}
		}
		else {
			return TrianglePointPair(triangle, *start);
		}
	}

	Line3D() {
		this->start = new glm::vec3(0.0f, 0.0f, 0.0f);
		this->end = new glm::vec3(0.0f, 0.0f, 0.0f);
		initGL();
	}

	~Line3D() {

	}
};