#pragma once
#include "GL/glew.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "config.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "image_loader.h"

struct v3 {
public:
	float x, y, z;

	v3::v3() {
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	v3::v3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float get(int i) {
		if (i == 0) {
			return this->x;
		}
		else if (i == 1) {
			return this->y;
		}
		else if (i == 2) {
			return this->z;
		}
	}
};

struct v2 {
public:
	float x, y;
	v2::v2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

class Material {
public:
	std::string name;
	float Ns;
	v3 Ka, Kd, Ks;
	int d, illum;
	std::string map_Kd;

public:
	Material::Material() {
		this->name = "null";
	}
	//TODO : use different materials in mtl file
	Material::Material(std::string parent, std::string path, std::string fileName) {
		std::ifstream file(parent + "/" + path + "/" + fileName);
		if (file.is_open()) {
			std::string word;
			while (file >> word) {
				if (word == "newmtl") {
					file >> word;
					name = word;
				}
				else if (word == "Ns") {
					file >> word; Ns = std::stof(word);
				}
				else if (word == "Ka") {
					file >> word; Ka.x = std::stof(word);
					file >> word; Ka.y = std::stof(word);
					file >> word; Ka.z = std::stof(word);
				}
				else if (word == "Kd") {
					file >> word; Kd.x = std::stof(word);
					file >> word; Kd.y = std::stof(word);
					file >> word; Kd.z = std::stof(word);
				}
				else if (word == "Ks") {
					file >> word; Ks.x = std::stof(word);
					file >> word; Ks.y = std::stof(word);
					file >> word; Ks.z = std::stof(word);
				}
				else if (word == "d") {
					file >> word; d = std::stoi(word);
				}
				else if (word == "illum") {
					file >> word; illum = std::stoi(word);
				}
				else if (word == "map_Kd") {
					file >> word; map_Kd = parent + "/" + path + "/" + word;
				}
			}
		}
		else std::cout << "material failed to load at path:" << parent + "/" + path + "/" + fileName << " - could not be opened" << std::endl;
		file.close();
	}

	Material::~Material() {

	}
};

class Object {
public:
	glm::mat4 projection, model;
private:
	GLuint common_textureID = 0;
public:
	Material* material;
	std::string name;
	std::string usemtl;
	std::string s;
	std::vector<v3> *vv, *vn, *fv, *ft, *fn;
	std::vector<v2> *vt;
private:
	unsigned int VBO, VAO;
	GLuint textureID;
	int vertSize = 0, indiceSize = 0;
	Shader* shader;
public:
	Object::Object(Material *&material, std::string name, std::string usemtl, std::string s, std::vector<v3> *&vv, std::vector<v3> *&vn, std::vector<v3> *&fv, std::vector<v3> *&ft, std::vector<v3> *&fn, std::vector<v2> *&vt, GLuint texture) {
		this->material = material;
		this->name = name;
		this->s = s;
		this->vv = vv;
		this->vn = vn;
		this->fv = fv;
		this->ft = ft;
		this->fn = fn;
		this->vt = vt;
		this->common_textureID = texture;
		projection = glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f);
		initGL();
		cleanUp();
	}

	void Object::initGL() {
		shader = new Shader("default.vs", "default.fs", nullptr);
		shader->use();
		shader->setMat4("projection", projection);

		vertSize = 3 * (3 + 2 + 3) * fv->size();
		indiceSize = 3 * fv->size();
		GLfloat* vertices = new GLfloat[vertSize];


		for (size_t i = 0; i < fv->size(); i++) {
			for (size_t j = 0; j < 3; j++) {
				vertices[i * 24 + j * 8 + 0] = vv->at(fv->at(i).get(j) - 1).x;
				vertices[i * 24 + j * 8 + 1] = vv->at(fv->at(i).get(j) - 1).y;
				vertices[i * 24 + j * 8 + 2] = vv->at(fv->at(i).get(j) - 1).z;

				vertices[i * 24 + j * 8 + 3] = vn->at(fn->at(i).get(j) - 1).x;
				vertices[i * 24 + j * 8 + 4] = vn->at(fn->at(i).get(j) - 1).y;
				vertices[i * 24 + j * 8 + 5] = vn->at(fn->at(i).get(j) - 1).z;

				vertices[i * 24 + j * 8 + 6] = vt->at(ft->at(i).get(j) - 1).x;
				vertices[i * 24 + j * 8 + 7] = vt->at(ft->at(i).get(j) - 1).y;
			}
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertSize, vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		delete[] vertices;

		if (!common_textureID) {
			textureID = ImageLoader::loadTexture(material->map_Kd.c_str());
		}
		else {
			textureID = common_textureID;
		}
	}

	void Object::move(glm::vec3 rotate, glm::vec3 translate) {
		glm::mat4 modelMatrix(1);
		modelMatrix = glm::translate(modelMatrix, translate);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = modelMatrix;
	}

	void Object::setShader(Shader*& shader) {
		delete this->shader;
		this->shader = shader;
	}

	void Object::render(glm::mat4 view) {
		shader->use();
		shader->setMat4("view", view);
		shader->setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, indiceSize);
	}

	void Object::render(glm::mat4 view, glm::vec3 translate) {
		shader->use();
		shader->setMat4("view", view);
		glm::mat4 model_t(1);
		model_t = glm::translate(model_t, translate);
		shader->setMat4("model", model_t);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, indiceSize);
	}

	void Object::cleanUp() {
		//vv->clear();
		//vt->clear();
		//vn->clear();
		fv->clear();
		ft->clear();
		fn->clear();
	}

	Object::~Object() {

	}
};