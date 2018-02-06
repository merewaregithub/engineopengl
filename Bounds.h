#pragma once
#include "Triangle3D.h"
#include <vector>
#include "Object.h"
#include <string>
#include "Camera.h"

class Bounds {
public:
	std::vector<Triangle3D*>* triangles;
public:
	std::string name;
	std::vector<v3> *vv, *vn, *fv, *fn;
	int vertSize = 0, indiceSize = 0;
public:
	Bounds::Bounds(std::string name, std::vector<v3> *&vv, std::vector<v3> *&vn, std::vector<v3> *&fv, std::vector<v3> *&fn) {
		this->name = name;
		this->vv = vv;
		this->vn = vn;
		this->fv = fv;
		this->fn = fn;
		triangles = new std::vector<Triangle3D*>();
		init();
		clean();
	}

	void Bounds::init() {
		vertSize = 3 * (3 + 2 + 3) * fv->size();
		indiceSize = 3 * fv->size();
		GLfloat* vertices = new GLfloat[vertSize];
		
		std::vector<glm::vec3> points{glm::vec3(), glm::vec3(), glm::vec3()};
		glm::vec3 normal;

		for (size_t i = 0; i < fv->size(); i++) {
			for (size_t j = 0; j < 3; j++) {
				points.at(j).x = vv->at(fv->at(i).get(j) - 1).x;
				points.at(j).y = vv->at(fv->at(i).get(j) - 1).y;
				points.at(j).z = vv->at(fv->at(i).get(j) - 1).z;

				normal.x = vn->at(fn->at(i).get(j) - 1).x;
				normal.y = vn->at(fn->at(i).get(j) - 1).y;
				normal.z = vn->at(fn->at(i).get(j) - 1).z;
			}
			triangles->push_back(new Triangle3D(points.at(0), points.at(1), points.at(2), normal));
		}
	
	}

	void Bounds::move(glm::vec3 rotate, glm::vec3 translate) {
		for (size_t i = 0; i < triangles->size(); i++) {
			triangles->at(i)->move(rotate, translate);
		}
	}

	void Bounds::render(glm::mat4 view) {
		for (size_t i = 0; i < triangles->size(); i++) {
			triangles->at(i)->render(view);
		}
	}

	void Bounds::cleanUp() {
		std::cout << "clean tri" << std::endl;
		triangles->clear();
	}

	void Bounds::clean() {
		fv->clear();
		fn->clear();
	}

	Bounds::~Bounds() {

	}
};