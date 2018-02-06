#pragma once
#include <vector>
#include "Model.h"
#include "glm\glm.hpp"
#include "Object.h"

class Animation {
public:
	Material* common_material;
	GLuint common_textureID = 0;
private:
	std::vector<Model*>* frames;
	int frameCount;
	int currentFrame;
public:
	Animation(std::string path, int frameCount) {
		common_material = new Material("animation", path, path + "_000001.mtl");
		common_textureID = ImageLoader::loadTexture(common_material->map_Kd.c_str());
		frames = new std::vector<Model*>();
		this->frameCount = frameCount;
		this->currentFrame = 0;
		for (size_t i = 1; i <= frameCount; i++) {
			frames->push_back(new Model("animation", path, + "_" + intFormat("000000", i), common_textureID, common_material));
		}
		
	}

	void Animation::tick() {
		currentFrame++;
		if (currentFrame >= frameCount) {
			currentFrame = 0;
		}
	}

	void Animation::move(glm::vec3 rotate, glm::vec3 translate) {
		frames->at(currentFrame)->move(rotate, translate);
	}

	void Animation::render(glm::mat4 view) {
		frames->at(currentFrame)->render(view);
	}

	Model*& Animation::getCurrentModel() {
		return frames->at(*&currentFrame);
	}

	void Animation::cleanUp() {
		frames->clear();
		delete frames;
	}

	std::string Animation::intFormat(std::string format, int number) {
		int digitCount = std::to_string(number).size();
		format = format.substr(0, format.size() - digitCount) + std::to_string(number);
		
		return format;
	}

	~Animation() {

	}
};