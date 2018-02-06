#pragma once
#include <vector>
#include "Model.h"
#include "Line3D.h"
#include "Camera.h"
#include "Animation.h"

class ModelPair {
public:
	TrianglePointPair tri_pair;
	Model* model;
public:
	ModelPair(TrianglePointPair tri_pair, Model *&model) {
		this->tri_pair = tri_pair;
		this->model = model;
	}
	ModelPair() {

	}
};

class Collision {
public:
	std::vector<Model*>* models;
	std::vector<Animation*>* animations;
	Triangle3D* tri = nullptr;
	Model* selectedModel = nullptr;
public:
	Collision() {
		this->models = new std::vector<Model*>();
		this->animations = new std::vector<Animation*>();
	}

	void Collision::add(Model *&model) {
		this->models->push_back(model);
	}

	void Collision::add(Animation *&animation) {
		this->animations->push_back(animation);
	}

	void Collision::checkMouseModelCollision() {
		if (tri != nullptr) {
			tri->polyMode = GL_LINE;
			//selectedModel->canRender = true;
		}

		std::vector<ModelPair> pairs;
		float lenght = 150.0f;

		for (size_t i = 0; i < models->size(); i++) {
			
			for (size_t j = 0; j < models->at(i)->bounds->triangles->size(); j++) {
				TrianglePointPair pair = Camera::mouse_sight->intersects(models->at(i)->bounds->triangles->at(j));
				//pair.triangle->polyMode = GL_LINE;
				if (pair.P != *Camera::mouse_sight->start) {
					pairs.push_back(ModelPair(pair, models->at(i)));
					break;
				}
			}

		}

		for (size_t i = 0; i < animations->size(); i++) {

			for (size_t j = 0; j < animations->at(i)->getCurrentModel()->bounds->triangles->size(); j++) {
				TrianglePointPair pair = Camera::mouse_sight->intersects(animations->at(i)->getCurrentModel()->bounds->triangles->at(j));
				//pair.triangle->polyMode = GL_LINE;
				if (pair.P != *Camera::mouse_sight->start) {
					pairs.push_back(ModelPair(pair, animations->at(i)->getCurrentModel()));
					break;
				}
			}

		}

		
		for (size_t i = 0; i < pairs.size(); i++) {
			float dis = glm::length(*Camera::mouse_sight->start - pairs.at(i).tri_pair.P);
			if (dis < lenght) {
				lenght = dis;
				tri = pairs.at(i).tri_pair.triangle;
				selectedModel = pairs.at(i).model;
			}
		}

		if (tri != nullptr) {
			if (pairs.size() == 0) {
				tri->polyMode = GL_LINE;
				//selectedModel->canRender = true;
			}
			else {
				tri->polyMode = GL_FILL;
				//selectedModel->canRender = false;
			}
		}
	}

	~Collision(){

	}
};