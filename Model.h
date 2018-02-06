#pragma once
#include <map>
#include <string>
#include <fstream>
#include "Object.h"

class Model {
public:
	GLuint common_textureID;
	Material* common_material;
private:
	std::string parent_folder;
	std::string model_number;
private:
	Material* material;
	std::string name;
	std::string usemtl;
	std::string s;
	std::vector<v3> *vv, *vn, *fv, *ft, *fn;
	std::vector<v2> *vt;
public:
	std::map<std::string, Object>* objects;
public:
	Model() {

	}

	Model(std::string parent, std::string path, std::string model_number, GLuint texture, Material *&mtl) {
		this->common_material = mtl;
		this->common_textureID = texture;
		this->parent_folder = parent;
		this->model_number = model_number;
		initModel(path);
	}

	Model(std::string path) {
		this->common_material = new Material();
		this->common_textureID = 0; //TODO : optimize common texture re-loading in model object
		parent_folder = "model";
		model_number = "";
		initModel(path);
	}

	void Model::initModel(std::string path) {
		objects = new std::map<std::string, Object>;
		vv = new std::vector<v3>();
		vt = new std::vector<v2>();
		vn = new std::vector<v3>();
		fv = new std::vector<v3>();
		ft = new std::vector<v3>();
		fn = new std::vector<v3>();

		bool seen_object = false;
		std::ifstream file(parent_folder + "/" + path + "/" + path + model_number + ".obj");
		if (file.is_open()) {
			std::string word;
			while (file >> word) {
				if (word == "v") {
					file >> word; float x = std::stof(word);
					file >> word; float y = std::stof(word);
					file >> word; float z = std::stof(word);
					vv->push_back(v3(x, y, z));
				}
				else if (word == "vt") {
					file >> word; float x = std::stof(word);
					file >> word; float y = std::stof(word);
					vt->push_back(v2(x, y));
				}
				else if (word == "vn") {
					file >> word; float x = std::stof(word);
					file >> word; float y = std::stof(word);
					file >> word; float z = std::stof(word);
					vn->push_back(v3(x, y, z));
				}
				else if (word == "f") {
					file >> word;
					float vv1 = std::stoi(word.substr(0, word.find_first_of('/')));
					float tt1 = std::stoi(word.substr(word.find_first_of('/') + 1, word.find_last_of('/') - word.find_first_of('/') + 1));
					float nn1 = std::stoi(word.substr(word.find_last_of('/') + 1, word.size() - word.find_last_of('/') + 1));

					file >> word;
					float vv2 = std::stoi(word.substr(0, word.find_first_of('/')));
					float tt2 = std::stoi(word.substr(word.find_first_of('/') + 1, word.find_last_of('/') - word.find_first_of('/') + 1));
					float nn2 = std::stoi(word.substr(word.find_last_of('/') + 1, word.size() - word.find_last_of('/') + 1));

					file >> word;
					float vv3 = std::stoi(word.substr(0, word.find_first_of('/')));
					float tt3 = std::stoi(word.substr(word.find_first_of('/') + 1, word.find_last_of('/') - word.find_first_of('/') + 1));
					float nn3 = std::stoi(word.substr(word.find_last_of('/') + 1, word.size() - word.find_last_of('/') + 1));

					fv->push_back(v3(vv1, vv2, vv3));
					ft->push_back(v3(tt1, tt2, tt3));
					fn->push_back(v3(nn1, nn2, nn3));
				}
				else if (word == "mtllib") {
					file >> word;
					if (common_material->name == "null") {
						material = new Material(parent_folder, path, word);
					}
				}
				else if (word == "o") {
					if (seen_object) {
						objects->insert(std::pair<std::string, Object>(name, Object(material, name, usemtl, s, vv, vn, fv, ft, fn, vt, common_textureID)));
					}
					else {
						seen_object = true;
					}
					file >> word;
					name = word;
				}
				else if (word == "usemtl") {
					file >> word;
					usemtl = word;
				}
				else if (word == "s") {
					file >> word;
					s = word;
				}
			}
			objects->insert(std::pair<std::string, Object>(name, Object(material, name, usemtl, s, vv, vn, fv, ft, fn, vt, common_textureID)));
		}
		else std::cout << "fail: " << parent_folder + "/" + path + "/" + path + model_number + ".obj" << " - could not be opened" << std::endl;
		file.close();

		cleanUp();
	}

	void Model::render(glm::mat4 view, glm::vec3 translate) {
		for (std::map<std::string, Object>::iterator iter = objects->begin(); iter != objects->end(); ++iter) {
			iter->second.render(view, translate);
		}
	}

	void Model::cleanUp() {
		vv->clear();
		vt->clear();
		vn->clear();
		fv->clear();
		ft->clear();
		fn->clear();

		delete vv;
		delete vt;
		delete vn;
		delete fv;
		delete ft;
		delete fn;
	}

	~Model() {

	}
};